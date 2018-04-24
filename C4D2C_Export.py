import c4d
from c4d import gui
from c4d import documents
import traceback
import os
import re
from enum import Enum
from abc import ABC, abstractmethod

header = 'C4D2C v1.0'
fieldsep = '\n'
valuesep = ','
scale = 1.0 / 100.0
pathdescformername = 'outpath.c4d2c'

class DataType(Enum):
    NATURAL = 0
    INT = 1
    REAL = 2
    
    def __add__(self, other):
        return Test(max(self.value, other.value))

    @staticmethod
    def detect_type(value):
        if isinstance(value, int):
            if value >= 0:
                return DataType.NATURAL
            else:
                return DataType.INT
        elif isinstance(value, float):
            return DataType.REAL
        else:
            return None

class Vector:

    def __init__(self, source = None):
        if source is None:
            self.data = list()
            self.type = DataType.NATURAL
        else:
            self.data = list(source.data)
            self.type = source.type
    
    def __getitem__(self, key):
        return self.data[key]

    def __len__(self):
        return len(self.data)

    def __add__(self, comp):
        clone = Vector(self) 
        clone += comp
        return clone

    def add_component(self, comp):
        comp_type = DataType.detect_type(comp)
        if comp_type is None:
            raise ValueError('Invalid component type')
        else:
            self.type = self.type + comp_type
            self.data.append(comp)

    def __iadd__(self, addend):
        if isinstance(addend, list):
            for comp in addend:
                add_component(comp)
        else:
            add_component(addend)
        return self

class Buffer(ABC):

    name_re = re.compile(r'[a-z_]+', re.IGNORECASE)

    def __init__(self):
        self.name = None

    def validate_name(self):
        if isinstance(self.name, int):
            if self.name < 0:
                return ValueError('Integer names cannot be negative')
        elif isinstance(self.name, str):
            if not Buffer.name_re.fullmatch(self.name):
                return ValueError('User defined names must match the regexp [A-Za-z_]')
        else:
            return TypeError('Names must be integers or strings')
        return None

    @abstractmethod
    def encode(self):
        pass
    
    
        




        
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
