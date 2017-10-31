from pysmt.shortcuts import Symbol, And, GE, LT, Plus, Equals, Int, get_model
from pysmt.typing import INT

hello = [Symbol(s, INT) for s in "animesh chandra pathak"]
world = [Symbol(s, INT) for s in "satyam pandey"]
letters = set(hello+world)
domains = And([And(GE(l, Int(1)),
                   LT(l, Int(1000))) for l in letters])

sum_hello = Plus(hello) # n-ary operators can take lists
sum_world = Plus(world) # as arguments
problem = And(Equals(sum_hello, sum_world),
              Equals(sum_hello, Int(125)))
formula = And(domains, problem)

print("Serialization of the formula:")
print(formula)

model = get_model(formula)
if model:
  print(model)
else:
  print("No solution found")
