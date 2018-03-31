import c4d
from c4d import gui
from c4d import documents
import traceback
import os

class Vertex:
    
    def __init__(self, pos = c4d.Vector(), norm = c4d.Vector()):
        self.pos = pos
        self.norm = norm
        
    def __eq__(self, other):
        return other.pos == self.pos and other.norm == self.norm
        
class Mesh:
    
    def __init__(self):
        self.verts = []
        self.inds = []
    
    def Add(self, vert):
        for i, exvert in enumerate(self.verts):
            if exvert == vert:
                # Duplicated vertex
                self.inds.append(i)
                return
        # New vertex
        self.inds.append(len(self.verts))
        self.verts.append(vert)
        
    def GetIndArray(self):
        return self.inds
        
    def MakePosNormArray(self, scale = 1.0):
        va = []
        for vert in self.verts:
            va.append(vert.pos.x * scale)
            va.append(vert.pos.y * scale)
            va.append(vert.pos.z * scale)
            va.append(vert.norm.x)
            va.append(vert.norm.y)
            va.append(vert.norm.z)
        return va  
      
                
def MakeMesh(obj):
    if not IsPolyObjTriangulated(obj):
        raise ValueError('Polygon is not triangulated')
    
    mesh = Mesh()
    
    polys = obj.GetAllPolygons()
    points = obj.GetAllPoints()
    norms = GetPolyObjNormals(obj)
    
    n = 0
    for poly in polys:
        mesh.Add(Vertex(points[poly.a], norms[n]))
        n += 1
        mesh.Add(Vertex(points[poly.b], norms[n]))
        n += 1
        mesh.Add(Vertex(points[poly.c], norms[n]))
        n += 1
        
    return mesh

def GetPolyObjNormals(obj):
    norms = obj.CreatePhongNormals()
    if norms is None:
        # Object has no Phong tag
        norms = []
        polys = obj.GetAllPolygons()
        for poly in polys:
            # Calculate normal for triangle
            u = poly.b - poly.a
            v = poly.c - poly.a
            norm = u.Cross(v)
            norms.add(norm)
    return norms
    
def IsPolyObj(obj):
    return obj.GetType() == c4d.Opolygon
    
def IsPolyObjTriangulated(obj):
        polys = obj.GetAllPolygons()
        for poly in polys:
            if not poly.IsTriangle():
                return False
        return True    
    
def ToSVString(array, separator):
    svs = ''
    for el in array:
        svs += str(el) + separator
    return svs.rstrip(separator)

def ToCArray(array, varname, vartype):
    vararr = 'const ' + vartype + ' ' + varname + '[] = {' + ToSVString(array,',') + '};'
    varnum = 'const int ' + varname + '_size = ' + len(array) + ';'
    return vararr + '\n' + varnum + '\n'

def ToCppArray(array, varname, vartype):
    return 'constexpr std::vector<' + vartype + '> ' + varname + '{' + ToSVString(array,',') + '};\n'

def main():
    unit = 100.0
    scale = 1.0 / unit
    base = 'C:/Users/zocch/Desktop/'
    
    doc = documents.GetActiveDocument()
    objs = doc.GetActiveObjects(c4d.GETACTIVEOBJECTFLAGS_CHILDREN)
    
    for obj in objs:
        if IsPolyObj(obj):
            name = obj.GetName()
            mesh = MakeMesh(obj)
            va = mesh.MakePosNormArray(scale)
            ia = mesh.GetIndArray()
            txt = '#include <vector>\n'
            txt += ToCppArray(va,name + '_verts','float')
            txt += ToCppArray(ia,name + '_inds','short')
            fname = os.path.join(base,name)
            f = open(fname,'w')
            f.write(txt)
            f.close()

if __name__=='__main__':
    try:
        main()
        gui.MessageDialog('Done')
    except Exception as e:
        traceback.print_exc()
        gui.MessageDialog(str(e))
        
