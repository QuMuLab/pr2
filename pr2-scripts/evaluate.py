

# WARNING: This script assumes that you are running things from inside the planner's docker container

import os, argparse, sys

# TODO: Accept json of configurations to run


DOMAINS = {}
COLLECTIONS = {}

MEMORY = 4096 # in Mb, and if changed, Dockerfile (planner calls) need changing too

ABLATION_SETTINGS = {
    'pr2-no-objsampling': "",
    'pr2-no-poisoning': " --deadend-poison-search 0",
    'pr2-no-fsap-penalty': " --weaksearch-penalize-potential-fsaps 0",
    'pr2-no-full-scd-marking': " --psgraph-full-scd-marking 0",
    'pr2-no-force-1safe': " --deadend-force-1safe-weak-plans 0",
}


CACHE_CODE = """
import glob, random, os
while True:
    problems = [p for p in glob.glob('*') if ('run.py' != p and 'tmp-' not in p)]
    if len(problems) == 0:
        print(f'\\n\\tAll done!')
        exit(0)
    problem = random.choice(problems)

    with open(problem, 'r') as f:
        cmdline = f.read()
    print(f'\\n\\tRunning: {cmdline}')

    os.system(f'rm {problem}')

    os.system(cmdline)
"""

def gen_code(planner,domprob):
    dom = domprob[0].split('/')[-2]
    prob = domprob[1].split('/')[-1].split('.')[0]
    return f"{planner}.{dom}__{prob}"

def generate_todo_stack(planner, domprobs, output, timeout):
    codes = {}
    for domprob in domprobs:
        code = gen_code(planner,domprob)

        if planner in ['mynd', 'paladinus']:
            memstring = ""
        else:
            memstring = f"ulimit -v {MEMORY*1024}; "

        if planner == 'pr2-no-objsampling':
            plannerstring = 'pr2 --disable-object-sampling'
        elif planner in ABLATION_SETTINGS:
            plannerstring = "pr2"
        else:
            plannerstring = planner

        if planner in ABLATION_SETTINGS:
            optionstring = ABLATION_SETTINGS[planner]
        else:
            optionstring = ""

        codes[code] = f"timeout {timeout} bash -c \"{memstring}(mkdir tmp-{code}; cd tmp-{code}; time {plannerstring} {domprob[0]} {domprob[1]}{optionstring}; cd ..) > {output}/{code}.out 2>&1\"; rm -rf tmp-{code}"
    return codes

def run(cmdline):
    os.system(cmdline)

def main(planner, benchmark, collection, output, cache, timeout):
    # Fetch the problems to run
    if benchmark:
        benchmarks = {benchmark: DOMAINS[benchmark]}
    elif collection:
        benchmarks = {bench: DOMAINS[bench] for bench in COLLECTIONS[collection]}

    # Generate the todo stack
    todo_stack = {}
    for bench in benchmarks:
        todo_stack[bench] = generate_todo_stack(planner, benchmarks[bench], output, timeout)

    # Check if we should cache
    if cache:
        # Make the cache directory if it doesn't exist
        if not os.path.exists(cache):
            os.makedirs(cache)
        # Write the run.py file
        with open(f'{cache}/run.py', 'w') as f:
            f.write(CACHE_CODE)
        # Write the individual files to run
        for bench in todo_stack:
            for code in todo_stack[bench]:
                with open(f"{cache}/{code}", 'w') as f:
                    f.write(todo_stack[bench][code])
    else:
        # Create the output directory if it doesn't exist
        if not os.path.exists(output):
            os.makedirs(output)
        # Run the benchmarks
        for bench in todo_stack:
            for code in todo_stack[bench]:
                run(todo_stack[bench][code])

if __name__ == "__main__":
    # Parse the command-line arguments
    parser = argparse.ArgumentParser(description="Evaluate a planner on a benchmark")

    # Must specify the catalogue location
    parser.add_argument("--catalogue", type=str, required=True, help="The location of the benchmark directory (should contain the catalogue file)")

    # Planners include pr2, prp, fond-sat, paladinus, mynd, the ablation variants of pr2, and meta-planners pr2-ablation + all-fond-planners
    parser.add_argument("--planner", required=True, help="The planner to run")

    # Force either benchmark or collection
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--benchmark", help="The benchmark to evaluate the planner on")
    group.add_argument("--collection", help="The collection of benchmarks to evaluate the planner on")

    # Resource limits
    parser.add_argument("--time-limit", type=int, default=3600, help="The time limit (s) for the planner")

    # Output directory
    parser.add_argument("--output", required=True, help="The output directory")

    # Optional cache directory
    parser.add_argument("--cache", help="If specified, planner calls will be cached")

    planner = parser.parse_args().planner
    benchmark = parser.parse_args().benchmark
    collection = parser.parse_args().collection

    time_limit = parser.parse_args().time_limit

    output = os.path.abspath(parser.parse_args().output)
    cache = parser.parse_args().cache

    # Import the catalogue python file
    cataloguef = os.path.abspath(parser.parse_args().catalogue)
    sys.path.append(cataloguef)
    import catalogue
    DOMAINS = catalogue.DOMAINS
    COLLECTIONS = catalogue.COLLECTIONS

    if planner == 'pr2-ablation':
        for ablation in list(ABLATION_SETTINGS.keys()) + ['pr2']:
            print(f'Scheduling {ablation}')
            main(ablation, benchmark, collection, output, cache, time_limit)
    elif planner == 'all-fond-planners':
        for fond_planner in ['fond-sat', 'paladinus', 'mynd', 'pr2', 'prp']:
            print(f'Scheduling {fond_planner}')
            main(fond_planner, benchmark, collection, output, cache, time_limit)
    else:
        main(planner, benchmark, collection, output, cache, time_limit)

