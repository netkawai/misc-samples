import geopandas as gpd
import pyvista as pv
import numpy as np

def lines_from_points(points):
    """Given an array of points, make a line set"""
    poly = pv.PolyData()
    poly.points = points
    cells = np.full((len(points)-1, 3), 2, dtype=np.int_)
    cells[:, 1] = np.arange(0, len(points)-1, dtype=np.int_)
    cells[:, 2] = np.arange(1, len(points), dtype=np.int_)
    poly.lines = cells
    return poly


plotter = pv.Plotter()
gdf = gpd.read_file('rings.dxf')
#gdf.plot()
for gem in gdf.geometry:
    polyline = lines_from_points(np.array(list(gem.coords)))
    tube = polyline.tube(radius=0.1)
    plotter.add_mesh(tube)
    
plotter.show()
#plotter.set_position([19157,50053,9392])