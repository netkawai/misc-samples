'''from tkinter import *'''
import tkinter as tk
from tkinter import font as ft

root = tk.Tk()

def callback(event):
  print("clicked at", event.x, event.y)

if __name__ == '__main__':
  frame = tk.Frame(root, width=200, height=150)
  tk.Label(frame, text="The quick brown fox jumps over the lazy dog.").pack()
  tk.Label(frame, text='三浦按針',font=ft.Font(family='Noto Sans',size=20)).pack()
  frame.bind("<Button-1>",callback)
  textArea = tk.Text(frame)
  textArea.pack()
  frame.pack()
  list_fonts = list(ft.families())
  for font in list_fonts:
    textArea.insert(tk.END,font+'\n')
  root.mainloop()
