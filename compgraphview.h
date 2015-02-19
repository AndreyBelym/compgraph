#ifndef COMPGRAPHVIEW_H
#define COMPGRAPHVIEW_H

#include <QGraphicsView>

//Размерность точки в однородных координатах
const double defN=-0.004;
const int N=4;
const int MaxColor=250;
enum Axes{
    OX,OY,OZ
};

//Мой собственный вектор(со всеми прилагающимися) для
//точки в однородных координатах
template <typename T> class VectorT {
    T *data;
public:
    int n;
    //Конструктор по декартовым координатам точки
    VectorT(int x,int y);
    VectorT(int x,int y,int z);
    //Конструктор заданной размерности
    VectorT(int n);
    VectorT(const VectorT<T>& other);
    VectorT();
    ~VectorT();
    //Ниасилил перегрузку typecast(если она есть)
    QPoint getPoint();
    VectorT& operator =(const VectorT& other);
    T& operator [](int n) const;
};

typedef VectorT<double> Vector;

typedef struct{
    int x,y;double z; bool next_y;
} SimplePoint;

template <typename T>
class MatrixT{
    VectorT<T> **data;
public:
    int n,m;
    //Конструктор заданной размерности
    MatrixT(int n,int m);
    ~MatrixT();
    VectorT<T>& operator [](int m) const;
    MatrixT& operator =(const MatrixT& other);
    //Перемножение матриц
    MatrixT operator *(const MatrixT& other);
    //Умножение на вектор. Учитывается однородный масштаб
    //Есть функция простого перемножения матрицы на вектор, ниже
    VectorT<T> operator *(const VectorT<T>& other);
};

typedef MatrixT<double> Matrix;

typedef QList<int> figure_t;
typedef QList<SimplePoint> points;
class CompGraphView : public QGraphicsView
{
    Q_OBJECT;
public:
    //Матрицы трансформаций - поворота и смещения/масштаба
    Matrix *ScaleMatrix,*RotOXMatrix,*RotOYMatrix,*RotOZMatrix,*MoveMatrix;
    //Знак Каспера
    QList<Vector> figure_points;
    QList<figure_t> figures;
    explicit CompGraphView(QWidget *parent = 0);

private:
    //Отрисовка
    void paintEvent(QPaintEvent *event);    
};

//Перемножение матриц
Matrix multMnM(const Matrix &a, const Matrix &b);
//Умножение матрицы на вектор
Vector multMnV(const Matrix &a,const Vector &v);
//Умножение матрицы на вектор с учетом однородного масштаба
Vector multMnVNorm(const Matrix &a,const Vector &v);

//Получение матрицы вращения
Matrix RotM(const double alpha=0,Axes axis=OY);
/*Получение матрицы смещения/масштаба
p,q - координаты смещения
scl_x,scl_y - масштаб по осям X и Y
scl_gen - однородный масштаб*/
Matrix MovM(const double p=0, const double q=0, const double r=0,
            const double scl_x=1, const double scl_y=1, const double scl_z=1,
            const double scl_gen=1,
            const double l=0, const double m=0, const double n=0);

template <typename T>
VectorT<T>::VectorT(int x, int y){
    this->n=3;
    data=new T[this->n];
    (*this)[0]=x;(*this)[1]=y;(*this)[2]=1;
}

template <typename T>
VectorT<T>::VectorT(int x, int y, int z){
    this->n=4;
    data=new T[this->n];
    (*this)[0]=x;(*this)[1]=y;(*this)[2]=z;(*this)[3]=1;
}

template <typename T>
VectorT<T>::VectorT(int n){
    this->n=n;
    data=new T[this->n];
}

template <typename T>
VectorT<T>::VectorT(const VectorT<T>& other){
    this->n=other.n;
    data=new T[this->n];
    for(int i=0;i<this->n;i++){
        this->data[i]=other.data[i];
    }
}

template <typename T>
VectorT<T>::VectorT(){
    this->n=0;
    this->data=NULL;
}

template <typename T>
VectorT<T>::~VectorT(){
    delete data;
}

template <typename T>
T& VectorT<T>::operator [](const int n) const{
    if(n>=0&&n<=this->n){
        return data[n];
    }
}

template <typename T>
QPoint VectorT<T>::getPoint(){
    return QPoint((*this)[0],(*this)[1]);
}

template <typename T>
VectorT<T>& VectorT<T>::operator =(const VectorT<T>& other){
    for(int i=0;i<this->n;i++){
        this->data[i]=other.data[i];
    }
    return *this;
}

template <typename T>
MatrixT<T>::MatrixT(int n,int m){
    this->n=n;
    this->m=m;
    data = new VectorT<T>*[m];
    for (int i=0;i<m;i++){
        data[i]=new VectorT<T>(n);
    }
}

template <typename T>
MatrixT<T>::~MatrixT(){
    for (int i=0;i<m;i++){
        delete data[i];
    }
    delete data;
}

template <typename T>
VectorT<T>& MatrixT<T>::operator [](const int m) const{
    if(m>=0&&m<=this->m){
        return *data[m];
    }
}

template <typename T>
MatrixT<T>& MatrixT<T>::operator =(const MatrixT<T>& other){
    for(int i=0;i<this->n;i++){
        for(int j=0;j<this->m;++j){
            *this->data[j]=*other.data[j];
        }
    }
    return *this;
}

template <typename T>
MatrixT<T> MatrixT<T>::operator *(const MatrixT<T>& other){
    return multMnM(*this,other);
}

template <typename T>
VectorT<T> MatrixT<T>::operator *(const VectorT<T> &other){
    return multMnVNorm(*this,other);
}

#endif // COMPGRAPHVIEW_H
