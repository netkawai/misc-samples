import geopandas as gpd
import pyvista as pv
import gemgis as gg
import numpy as np

gdf = gpd.read_file('/home/kawai/lines.dxf')
gdf_lines = gg.vector.extract_xy(gdf)
vertices = gdf_lines[['X', 'Y', 'Z']].values
vertices
p = pv.Plotter(notebook=True)
sargs = dict(fmt="%.0f", color='black')

for i in range(0, len(vertices)//2 - 1):
    p.add_mesh(pv.Line(vertices[2*i],vertices[2*i+1]),scalar_bar_args=sargs)

p.set_background('white')
p.show_grid(color='black')
p.show(jupyter_backend='ipygany')