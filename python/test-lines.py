import sys
import ezdxf

doc = ezdxf.new('R12')
layer_r1_FS_BS = doc.layers.new('layer_r1_FS_BS')
layer_r1_LS_RS = doc.layers.new('layer_r1_RS_LS')
layer_r2_FS_BS = doc.layers.new('layer_r2_FS_BS')
layer_r2_LS_RS = doc.layers.new('layer_r2_RS_LS')


msp = doc.modelspace()
msp.add_line((0, 1, 1), (0, -1, 1), dxfattribs={'layer': 'layer_r1_FS_BS'})
msp.add_line((1, 0, 1), (-1, 0, 1), dxfattribs={'layer': 'layer_r1_RS_LS'})
msp.add_line((0, 2, 1), (0, 4, 1), dxfattribs={'layer': 'layer_r2_FS_BS'})
msp.add_line((1.5, 1, 1), (-1.5, 0, 1), dxfattribs={'layer': 'layer_r2_RS_LS'})
doc.saveas('test.dxf')