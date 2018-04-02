import c4d
from c4d import gui
from c4d import documents
import traceback
import os

header = 'C4D2C v1.0'
fieldsep = '\n'
valuesep = ','
scale = 1.0 / 100.0
pathdescformername = 'outpath.c4d2c'

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
    mesh = Mesh()
    polys = obj.GetAllPolygons()
    points = obj.GetAllPoints()
    phongnorms = obj.CreatePhongNormals()

    n = 0
    for poly in polys:
        if not poly.IsTriangle():
            raise ValueError('Polygon is not triangulated')

        a = points[poly.a]
        b = points[poly.b]
        c = points[poly.c]

        if phongnorms is not None:
            anorm = phongnorms[n + 0]
            bnorm = phongnorms[n + 1]
            cnorm = phongnorms[n + 2]
            n += 4
        else:
            norm = CalcTriangleNormal(a,b,c)
            anorm = bnorm = cnorm = norm

        mesh.Add(Vertex(a * scale, anorm))
        mesh.Add(Vertex(b * scale, bnorm))
        mesh.Add(Vertex(c * scale, cnorm))

    return mesh

def CalcTriangleNormal(a, b, c):
    u = b - a
    v = c - a
    norm = u.Cross(v)
    return norm.GetNormalized()  

def SerializeVec(v):
    return str(v.x) + valuesep + str(v.y) + valuesep + str(v.z) + valuesep

def SerializePolyObj(obj):
    if obj.GetType() != c4d.Opolygon:
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
    print('C4D2C v1.0 exporter script by Francesco Zoccheddu')

    doc = documents.GetActiveDocument()
    docpath = doc.GetDocumentPath()
    outpath = None

    pathdescname = os.path.join(docpath, pathdescformername)
    if (os.path.isfile(pathdescname)):
        print('Found path descriptor "' + pathdescname + '"')
        try:
            pathdescfile = open(pathdescname, 'r')
            pathdesc = pathdescfile.read()
            if not os.path.isabs(pathdesc):
                pathdesc = os.path.join(docpath, pathdesc)
            if os.path.isdir(pathdesc):
                outpath = pathdesc
                print('Using output path "' + outpath + '"')
            else:
                print('Invalid output path descriptor')
            pathdescfile.close()
        except IOError:
            print('IO Error while reading path descriptor')

    allok = True
    objs = doc.GetActiveObjects(c4d.GETACTIVEOBJECTFLAGS_CHILDREN)
    for obj in objs:
        try:
            ser = SerializePolyObj(obj)
            if outpath is None:
                dialogtitle = 'Export "' + obj.GetName() + '" object'
                filename = c4d.storage.SaveDialog(c4d.FILESELECTTYPE_ANYTHING, dialogtitle, "", docpath)
                if filename is None:
                    print('Object "' + obj.GetName() + '" export cancelled')
                    continue
            else:
                filename = os.path.join(outpath, obj.GetName())
            outfile = open(filename, 'w')
            outfile.write(ser)
            outfile.close()
            print('Object "' + obj.GetName() + '" exported successfully to "' + filename + '"')
        except ValueError as e:
            allok = False
            print('Error while exporting object "' + obj.GetName() + '": ' + str(e))
    
    print('Exporting process finished successfully')
    
    if allok:
        gui.MessageDialog('No error occurred')
    else:
        gui.MessageDialog('Some errors occured')


if __name__=='__main__':
    main()        
