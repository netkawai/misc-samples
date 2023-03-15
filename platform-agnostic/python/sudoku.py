def cross(A,B):
  "Cross product of elements in A and elements"
  return [ a+b for a in A for b in B]
digits = '123456789'
rows = 'ABCDEFGHI'
cols = digits
squares = cross(rows, cols)
print (squares)

'''
---------
-----3-85
--1-2----
---5-7---
--4---1--
-9-------
5------73
--2-1----
----4---9
'''
