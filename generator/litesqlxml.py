import lxml.etree as etree

def prepareRelation(e):
    if e.get('name') == None:
        relates = e.findall('relate')

        def getid(x):
            return x.get('object', x.get('interface'))

        def cmpfunc(x, y):
            return getid(x) < getid(y)
                
        relates.sort(cmpfunc)                                     
        
        e.set("name", "".join(map(getid, relates)) 
                       + e.get('id', '')
                       + "Relation")


def prepare(doc):
    root = doc.getroot()
    for el in root.iterchildren():
        if el.tag == 'relation':
            prepareRelation(el)

