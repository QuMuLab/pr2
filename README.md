# PR2

PR2 is the next iteration in the evolution of SoA non-deterministic planning.

## ![siren image](https://img.icons8.com/color/48/000000/warning-shield.png) WARNING ![siren image](https://img.icons8.com/color/48/000000/warning-shield.png)

This is a rewrite of the PR2 planner, described in detail over [[here](https://mulab.ai/project/pr2/)]. *It is non-operational!!!*

The purpose of this repository is to house the eventual re-working of the PR2 planner so that it maintains working operability with the latest version of FastDownward. This repository is a fork of the [FastDownward repository](https://github.com/aibasel/downward), and will be updated regularly. Until this worning message is removed, the code found here should *not* be considered operational.

What follows is the original README from the PR2 release. The FastDownward README can be found at [FD-README.md](FD-README.md).


## Docker Usage

To build the docker image where things should "just work":

```bash
docker build -t pr2 .
```

To run the docker image and mount the local directory as a volume:

```bash
docker run --cpus 1 -it -v $(pwd):/PROJECT pr2
```

## Planners Available

To make evaluations easier, pre-compiled/configured versions of all leading FOND planners are included. Currently, the list includes:

* [PRP](https://github.com/QuMuLab/planner-for-relevant-policies)
* [MyND](https://github.com/robertmattmueller/myND)
* [FONDSAT](https://github.com/tomsons22/FOND-SAT)
* [Paladinus](https://github.com/ramonpereira/paladinus)

Every effort was made to patch and configure each planner to their full potential (e.g., fixing parse errors with MyND and using a stronger SAT solver for FONDSAT). If there is any issue you would like to see fixed, or FOND planner you would like to see included, just [reach out](http://www.haz.ca).

## Evaluations

### Running the evaluations

All of the evaluation settings can be found by running the `evaluate.py` script:

```bash
python pr2-scripts/evaluate.py --help
```

**Note**: It assumes that you are in the `pr2` docker container to run things.

If you have multiple cores available to run evaluations, you can "cache" the solver calls, and run several threads simultaneously to solve problems from the queue. As an example, the following would...

* queue up solving for both the PR2 and PRP planners
* use the `all-fond-papers` collection
* store results in the `RESULTS/` folder
* store the cache of solver calls in the `CACHE/` folder

```bash
python pr2-scripts/evaluate.py --catalogue /PROJECT/fond-benchmarks/ --collection all-fond-papers --output RESULTS --cache CACHE --planner pr2

python pr2-scripts/evaluate.py --catalogue /PROJECT/fond-benchmarks/ --collection all-fond-papers --output RESULTS --cache CACHE --planner prp
```
Then, to start a thread of solving, navigate to the `CACHE/` directory and run the script:

```bash
cd CACHE
python run.py
```

To monitor the progress of the evaluations, run the following command (assumes that the CACHE is being used by several threads and that the results are in the `RESULTS` directory):

```bash
watch 'echo "scale=1 ; 100 * `ls -l RESULTS | wc -l` / (`ls -l CACHE | wc -l` + `ls -l RESULTS | wc -l`)" | bc'
```

### Analyzing the results

All of the analysis exists in a jupyter notebook found in the `pr2-scripts` directory. You can run the notebook by loading a dockerized jupyter notebook server from the top-level directory of this repository:

```bash
docker run -it --rm -p 8888:8888 -v "${PWD}":/home/jovyan/work jupyter/datascience-notebook:9e63909e0317
```
