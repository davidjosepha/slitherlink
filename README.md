## run slitherlink solver
$ ./slsolver mypuzzle.slk anotherpuzzle.slk

## proposed run-time
O((mn)^(2d+1))
where d is the maximum allowed depth

Depth 0 (no guessing): 0.610963 seconds


## analytic run-time

####Run time on 9 puzzles (varying sizes and solvable up to depth 2) on our on lab computer:

Depth 0: 0.610963 seconds (solved 1 puzzle)

Depth 1: 174.177 seconds (solved 3 puzzles)

Depth 2: 405.207 seconds (solved 9 puzzles)


####Run time on puzzle 9 (30x25):

Depth 0: 0.183561 seconds (<50% completed)

Depth 1: 66.0678 seconds (>75% completed)

Depth 2: 138.411 seconds (100% completed)


####Run Time on empty 3x3 puzzle (has to make every possible combination of guesses):

Depth 0: 0.001111

Depth 1: 0.033743

Depth 2: 1.19998

Depth 3: 57.2004

Depth 4: 2587.52

## Docs we're using

[Literature](https://docs.google.com/a/carleton.edu/document/d/1JIbNOp1rgz-ut7pTf2QnSlT59Fw72w0DB6UZx1eAnsY/edit?usp=sharing)

[Rules](https://docs.google.com/a/carleton.edu/spreadsheets/d/16aAdbXlpoZenO36wuw4UUrY4pavIWqwWaNs64sWjOZQ/edit?usp=sharing)

[Condradictions](https://docs.google.com/spreadsheets/d/1C7qa3c5CUIdbZEeCJoGSomTTQenytkeeJEjoGnCCqrA/edit?usp=sharing)

[Project Proposal](https://docs.google.com/document/d/1DoqF_4Fd1dm_CH8-3Pqbv8TbNHXmNZlnpfe8aXd2Io4/edit?usp=sharing)

[Function Guide](https://docs.google.com/a/carleton.edu/document/d/1tHmxpug5pisl-ebQF6S27yAUKyQ7hj6kyYq9syouwXg/edit?usp=sharing)
