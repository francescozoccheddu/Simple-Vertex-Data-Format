import c4d
from c4d import gui
from c4d import documents
import traceback
import os

header = 'C4D2GL v1.0'
fieldsep = '\n'
valuesep = ','
scale = 1.0 / 100.0

class Vertex:
    
    def __init__(self, pos, norm):
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
                self.inds.append(i)
                return
        self.inds.append(len(self.verts))
        self.verts.append(vert)
      
                
def MakeMesh(obj):
    if not IsPolyObjTriangulated(obj):
        raise ValueError('Polygon is not triangulated')
    
    mesh = Mesh()
    polys = obj.GetAllPolygons()
    points = obj.GetAllPoints()
    norms = GetPolyObjNormals(obj)
    
    n = 0
    for poly in polys:
        mesh.Add(Vertex(points[poly.a] * scale, norms[n]))
        n += 1
        mesh.Add(Vertex(points[poly.b] * scale, norms[n]))
        n += 1
        mesh.Add(Vertex(points[poly.c] * scale, norms[n]))
        n += 1
        
    return mesh

def CalcTriangleNormal(a, b, c):
    u = b - a
    v = c - a
    norm = u.Cross(v)
    return norm.GetNormalized()

def GetPolyObjNormals(obj):
    norms = obj.CreatePhongNormals()
    if norms is None:
        norms = []
        polys = obj.GetAllPolygons()
        for poly in polys:
            norms.add(CalcTriangleNormal(poly.a, poly.b, poly.c))
    return norms
    
def IsPolyObj(obj):
    return obj.GetType() == c4d.Opolygon
    
def IsPolyObjTriangulated(obj):
        polys = obj.GetAllPolygons()
        for poly in polys:
            if not poly.IsTriangle():
                return False
        return True    

def SerializeVec(v):
    return str(v.x) + valuesep + str(v.y) + valuesep + str(v.z) + valuesep

def SerializePolyObj(obj):
    if not IsPolyObj(obj):
        raise ValueError('Object is not a PolygonObject')
    mesh = MakeMesh(obj)
    ser = header
    ser += fieldsep
    ser += str(len(mesh.verts * 6))
    ser += fieldsep
    ser += str(len(mesh.inds))
    ser += fieldsep
    for v in mesh.verts:
        ser += SerializeVec(v.pos) + SerializeVec(v.norm)
    ser += fieldsep
    for i in mesh.inds:
        ser += str(i) + valuesep
    ser += fieldsep
    return ser
    
def main():
    print('C4D2GL v1.0 exporter script by Francesco Zoccheddu')

    doc = documents.GetActiveDocument()
    path = doc.GetDocumentPath()
    objs = doc.GetActiveObjects(c4d.GETACTIVEOBJECTFLAGS_CHILDREN)
    
    print('Exporting to path "' + path + '"')
    allok = True
    for obj in objs:
        try:
            ser = SerializePolyObj(obj)
            filename = os.path.join(path, obj.GetName())
            outfile = open(filename, 'w')
            outfile.write(ser)
            outfile.close()
            print('Object "' + obj.GetName() + '" exported successfully')
        except ValueError as e:
            allok = False
            print('Exception while exporting object "' + obj.GetName() + '": ' + str(e))
    
    print('Exporting process finished successfully')
    
    if allok:
        gui.MessageDialog('No error occurred')
    else:
        gui.MessageDialog('Some errors occured')


if __name__=='__main__':
    main()        
