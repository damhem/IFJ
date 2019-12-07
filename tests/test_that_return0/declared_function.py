
def foo(a,b,c):
    a=bar(a)

def ha():
    return 5

def bar(a):
    k = a + 2
    h = ha()
    return k

foo(1,2,3)
