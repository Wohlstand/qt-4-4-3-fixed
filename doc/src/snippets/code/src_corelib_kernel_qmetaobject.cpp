//! [0]
class MyClass
{
    Q_OBJECT
    Q_CLASSINFO("author", "Sabrina Schweinsteiger")
    Q_CLASSINFO("url", "http://doc.moosesoft.co.uk/1.0/")

public:
    ...
};
//! [0]


//! [1]
QByteArray normType = QMetaObject::normalizedType(" int    const  *");
// normType is now "const int*"
//! [1]


//! [2]
QMetaObject::invokeMethod(pushButton, "animateClick",
                          Qt::QueuedConnection);
//! [2]


//! [3]
QMetaObject::invokeMethod: Unable to handle unregistered datatype 'MyType'
//! [3]


//! [4]
QString retVal;
QMetaObject::invokeMethod(obj, "compute", Qt::DirectConnection,
                          Q_RETURN_ARG(QString, retVal),
                          Q_ARG(QString, "sqrt"),
                          Q_ARG(int, 42),
                          Q_ARG(double, 9.7));
//! [4]


//! [5]
class MyClass
{
    Q_OBJECT
    Q_CLASSINFO("author", "Sabrina Schweinsteiger")
    Q_CLASSINFO("url", "http://doc.moosesoft.co.uk/1.0/")

public:
    ...
};
//! [5]


//! [propertyCount]
const QMetaObject* metaObject = obj->metaObject();
QStringList properties;
for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
    properties << QString::fromLatin1(metaObject->property(i).name());
//! [propertyCount]


//! [methodCount]
const QMetaObject* metaObject = obj->metaObject();
QStringList methods;
for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
    methods << QString::fromLatin1(metaObject->property(i).signature());
//! [methodCount]
