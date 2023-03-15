from ipywidgets import interact

@interact(x=(0, 100, 10))
def p(x=50):
    pass