
import argparse, random

from fondparser.parser import Problem
from fondparser.formula import And


def get_symmetric_objects(problem: Problem):

    signatures = {}

    # Compute a unique problem file signature for each of the objects that aren't constants
    for o in (problem.objects - problem.constants):
        sig = list(problem.obj_to_type[o])
        for pred in problem.init.args:
            if o in [t[0] for t in pred.predicate.args]:
                sig.append(str(pred).replace(f'{o},', 'SKOLEMOBJ,').replace(f'{o})', 'SKOLEMOBJ)'))

        if isinstance(problem.goal, And):
            goal = problem.goal.args
        else:
            goal = [problem.goal]

        # If it's mentioned in the goal at all, forget about it just to be safe
        for pred in goal:
            if o in [t[0] for t in pred.predicate.ground_args]:
                sig.append(str(random.random()))

        signatures[o] = '/'.join(sorted(sig))

    # Invert to map the signature to the list of symmetric objects
    sig_to_obj = {}
    for o in signatures:
        sig = signatures[o]
        if sig not in sig_to_obj:
            sig_to_obj[sig] = []
        sig_to_obj[sig].append(o)

    # Return the lists of objects containing more than one
    return [objs for objs in sig_to_obj.values() if len(objs) > 1]

def remove_objects(prob, objs):

    # type_to_obj
    for t in prob.type_to_obj:
        for o in objs:
            if o in prob.type_to_obj[t]:
                prob.type_to_obj[t].remove(o)

    # obj_to_type and objects
    for o in objs:
        del prob.obj_to_type[o]
        prob.objects.remove(o)

    # init
    toremove = set()
    for pred in prob.init.args:
        for o in objs:
            if o in [t[0] for t in pred.predicate.args]:
                toremove.add(str(pred))

    prob.init.args = [p for p in prob.init.args if str(p) not in toremove]


def main(dom, prob, outf, sample):

    # Don't even bother if we have quantified effects
    with open(dom, 'r') as f:
        contents = f.read()
    if '(forall' in contents or '(exists' in contents:
        print(False)
        return

    prob = Problem(dom, prob)

    symmetric_objects = get_symmetric_objects(prob)

    # Used to just check if there are any symmetries
    if sample == -1:
        print(len(symmetric_objects) != 0)
        return

    # print()
    # for objs in symmetric_objects:
    #     print(f'\t({len(objs)}): {objs}')
    # print()

    objs_to_remove = []
    for objlist in symmetric_objects:
        objs_to_remove.extend(objlist[sample:])
    remove_objects(prob, objs_to_remove)

    with open(outf, 'w') as f:
        prob._export_problem(f)


if __name__ == '__main__':

    # Parse arguments
    parser = argparse.ArgumentParser(description='Sub-sample symmetric objects')

    ## --sample
    parser.add_argument('--sample', type=int, default=1, help='sample size', required=True)

    ## --domain
    parser.add_argument('--domain', type=str, default='', help='domain file', required=True)

    ## --problem
    parser.add_argument('--problem', type=str, default='', help='problem file', required=True)

    ## --output
    parser.add_argument('--output', type=str, default='', help='output problem file', required=True)

    args = parser.parse_args()

    main(args.domain, args.problem, args.output, args.sample)
