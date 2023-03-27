#ifndef _ANGLETRANSFORM_H_
#define _ANGLETRANSFORM_H_

#include<cmath>
#include<iostream>
using namespace std;
/*
实现旋转API，去掉Windows平台依赖，适应跨平台需要，正式使用时可以加上一个名字空间Han
有限维可交换Lie群：
一维可交换Lie群——用矩阵（线性变换）的形式来表示绕固定轴的旋转群SO(2)：
{{x'},{y'}}={{cosθ,-sinθ},{sinθ,cosθ}}{{x},{y}}={{xcosθ-ysinθ},{xsinθ+ycosθ}}
一个三维紧致非单连通不可交换单Lie群——三维转动群SO(3)也称为特殊正交群。三维空间绕固定点的一个转动，习惯用Euler角（θ，Φ，Ψ）来描述一个转动。不是单连通的流形。
g^Ψ_z表示绕z轴转Ψ角
g^θ_x表示绕x轴转θ角
g^Φ_y表示绕y轴转Φ角
于是g=g^Φ_yg^θ_xg^Ψ_z
令θ，Φ=0，即绕固定点O和固定轴z轴旋转Ψ
则{{x'},{y'}}={{cosθ,-sinθ},{sinθ,cosθ}}{{x},{y}}={{xcosθ-ysinθ},{xsinθ+ycosθ}}
{{x'},{y'},{z'}}={{cosΨ,-sinΨ,0},{sinΨ,cosΨ,0},{0,0,1}}{{x},{y},{z}}={{xcosΨ-ysinΨ},{xsinΨ+ycosΨ},{z}}
仅满足g^tg=gg^t[而不要求det g>0]的线性变换所构成的群称为O(3)——正交群。
三维转动群SO(3)是3维连通紧致单线性Lie群，相应的实Lie代数so(3)是3维1秩紧致实单Lie代数。
二维Lorentz群SO(2,1)是3维非紧致单线性Lie群，相应的实Lie代数so(2,1)是3维1秩非紧致实单Lie代数。
*/
typedef struct _D3DMATRIX
{
    union {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
}D3DMATRIX;
typedef float SCALAR;
//
// A 3D vector
//
class D3DVECTOR
{
public:
    SCALAR x,y,z; //x,y,z coordinates
public:
    D3DVECTOR() : x(0), y(0), z(0) {}
    D3DVECTOR( const SCALAR& a, const SCALAR& b, const SCALAR& c ) : x(a), y(b), z(c) {}
    //index a component
    //NOTE: returning a reference allows
    //you to assign the indexed element
    SCALAR& operator [] ( const long i )
    {
        return *((&x) + i);
    }
    //compare
    const bool operator == ( const D3DVECTOR& v ) const
    {
        return (v.x==x && v.y==y && v.z==z);
    }
    const bool operator != ( const D3DVECTOR& v ) const
    {
        return !(v == *this);
    }
    //negate
    const D3DVECTOR operator - () const
    {
        return D3DVECTOR( -x, -y, -z );
    }
    //assign
    const D3DVECTOR& operator = ( const D3DVECTOR& v )
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    //increment
    const D3DVECTOR& operator += ( const D3DVECTOR& v )
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
        return *this;
    }
    //decrement
    const D3DVECTOR& operator -= ( const D3DVECTOR& v )
    {
        x-=v.x;
        y-=v.y;
        z-=v.z;
        return *this;
    }
    //self-multiply
    const D3DVECTOR& operator *= ( const SCALAR& s )
    {
        x*=s;
        y*=s;
        z*=s;
        return *this;
    }
    //self-divide
    const D3DVECTOR& operator /= ( const SCALAR& s )
    {
        const SCALAR r = 1 / s;
        x *= r;
        y *= r;
        z *= r;
        return *this;
    }
    //add
    const D3DVECTOR operator + ( const D3DVECTOR& v ) const
    {
        return D3DVECTOR(x + v.x, y + v.y, z + v.z);
    }
    //subtract
    const D3DVECTOR operator - ( const D3DVECTOR& v ) const
    {
        return D3DVECTOR(x - v.x, y - v.y, z - v.z);
    }
    //post-multiply by a scalar
    const D3DVECTOR operator * ( const SCALAR& s ) const
    {
        return D3DVECTOR( x*s, y*s, z*s );
    }
    //pre-multiply by a scalar
    friend inline const D3DVECTOR operator * ( const SCALAR& s, const D3DVECTOR& v )
    {
        return v * s;
    }
    //divide
    const D3DVECTOR operator / (SCALAR s) const
    {
        s = 1/s;
        return D3DVECTOR( s*x, s*y, s*z );
    }
    //cross product
    const D3DVECTOR cross( const D3DVECTOR& v ) const
    {
        //Davis, Snider, "Introduction to Vector Analysis", p. 44
        return D3DVECTOR( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x );
    }
    //scalar dot product
    const SCALAR dot( const D3DVECTOR& v ) const
    {
        return x*v.x + y*v.y + z*v.z;
    }
    //length
    const SCALAR length() const
    {
        return (SCALAR)sqrt( (double)this->dot(*this) );
    }
    //unit vector
    const D3DVECTOR unit() const
    {
        return (*this) / length();
    }
    //make this a unit vector
    void normalize()
    {
        (*this) /= length();
    }
    //equal within an error 慹?
    const bool nearlyEquals( const D3DVECTOR& v, const SCALAR e ) const
    {
        return fabs(x-v.x)<e && fabs(y-v.y)<e && fabs(z-v.z)<e;
    }
};
typedef D3DVECTOR D3DXVECTOR3;
//
// A 4D vector
//
class D3DXVECTOR4
{
public:
    SCALAR x,y,z,w; //x,y,z,w coordinates
public:
    D3DXVECTOR4() : x(0), y(0), z(0),w(0) {}
    D3DXVECTOR4( const SCALAR& a, const SCALAR& b, const SCALAR& c , const SCALAR& d) : x(a), y(b), z(c), w(d) {}
    //index a component
    //NOTE: returning a reference allows
    //you to assign the indexed element
    SCALAR& operator [] ( const long i )
    {
        return *((&x) + i);
    }
    //compare
    const bool operator == ( const D3DXVECTOR4& v ) const
    {
        return (v.x==x && v.y==y && v.z==z && v.w==w);
    }
    const bool operator != ( const D3DXVECTOR4& v ) const
    {
        return !(v == *this);
    }
    //negate
    const D3DXVECTOR4 operator - () const
    {
        return D3DXVECTOR4( -x, -y, -z ,-w);
    }
    //assign
    const D3DXVECTOR4& operator = ( const D3DXVECTOR4& v )
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
        return *this;
    }
    //increment
    const D3DXVECTOR4& operator += ( const D3DXVECTOR4& v )
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
        w+=v.w;
        return *this;
    }
    //decrement
    const D3DXVECTOR4& operator -= ( const D3DXVECTOR4& v )
    {
        x-=v.x;
        y-=v.y;
        z-=v.z;
        w-=v.w;
        return *this;
    }
    //self-multiply
    const D3DXVECTOR4& operator *= ( const SCALAR& s )
    {
        x*=s;
        y*=s;
        z*=s;
        w*=s;
        return *this;
    }
    //self-divide
    const D3DXVECTOR4& operator /= ( const SCALAR& s )
    {
        const SCALAR r = 1 / s;
        x *= r;
        y *= r;
        z *= r;
        w *= r;
        return *this;
    }
    //add
    const D3DXVECTOR4 operator + ( const D3DXVECTOR4& v ) const
    {
        return D3DXVECTOR4(x + v.x, y + v.y, z + v.z,w+v.w);
    }
    //subtract
    const D3DXVECTOR4 operator - ( const D3DXVECTOR4& v ) const
    {
        return D3DXVECTOR4(x - v.x, y - v.y, z - v.z,w-v.w);
    }
    //post-multiply by a scalar
    const D3DXVECTOR4 operator * ( const SCALAR& s ) const
    {
        return D3DXVECTOR4( x*s, y*s, z*s ,w*s);
    }
    //pre-multiply by a scalar
    friend inline const D3DXVECTOR4 operator * ( const SCALAR& s, const D3DXVECTOR4& v )
    {
        return v * s;
    }
    //divide
    const D3DXVECTOR4 operator / (SCALAR s) const
    {
        s = 1/s;
        return D3DXVECTOR4( s*x, s*y, s*z ,s*w);
    }
    ////cross product，没有4维叉积
    //
    // const VECTOR cross( const VECTOR& v ) const
    // {
    //  //Davis, Snider, "Introduction to Vector Analysis", p. 44
    //  return VECTOR( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x );
    // }
    //scalar dot product
    const SCALAR dot( const D3DXVECTOR4& v ) const
    {
        return x*v.x + y*v.y + z*v.z+w*v.w;
    }
    //length
    const SCALAR length() const
    {
        return (SCALAR)sqrt( (double)this->dot(*this) );
    }
    //unit vector
    const D3DXVECTOR4 unit() const
    {
        return (*this) / length();
    }
    //make this a unit vector
    void normalize()
    {
        (*this) /= length();
    }
    //equal within an error 慹?
    const bool nearlyEquals( const D3DXVECTOR4& v, const SCALAR e ) const
    {
        return fabs(x-v.x)<e && fabs(y-v.y)<e && fabs(z-v.z)<e && fabs(w-v.w)<e;
    }
};
//add by Ivan_han 20130811
typedef D3DXVECTOR4 D3DXQUATERNION;
//由旋转矩阵创建四元数
inline D3DXQUATERNION CQuaternion(const _D3DMATRIX& m)
{
    D3DXQUATERNION ret;
    float tr, s, q[4];
    int i, j, k;
    int nxt[3] = {1, 2, 0 };
    // 计算矩阵的迹
    tr = m._11 + m._22 + m._33;
    // 检查矩阵迹是正还是负
    if(tr>0.0f)
    {
        s = sqrt(tr + 1.0f);
        ret.w = s / 2.0f;
        s = 0.5f / s;
        ret.x = (m._23 - m._32) * s;
        ret.y = (m._31 - m._13) * s;
        ret.z = (m._12 - m._21) * s;
    }
    else
    {
        // 迹是负
        // 寻找m11 m22 m33中的最大分量
        i = 0;
        if(m.m[1][1]>m.m[0][0]) i = 1;
        if(m.m[2][2]>m.m[i][i]) i = 2;
        j = nxt[i];
        k = nxt[j];
        s = sqrt((m.m[i][i] - (m.m[j][j] + m.m[k][k])) + 1.0f);
        q[i] = s * 0.5f;
        if( s!= 0.0f) s = 0.5f / s;
        q[3] = (m.m[j][k] - m.m[k][j]) * s;
        q[j] = (m.m[i][j] - m.m[j][i]) * s;
        q[k] = (m.m[i][k] - m.m[k][i]) * s;
        ret.x = q[0];
        ret.y = q[1];
        ret.z = q[2];
        ret.w = q[3];
    }
    return ret;
};
//由欧拉角创建四元数
inline D3DXQUATERNION CQuaternion(const D3DVECTOR& angle)
{
    D3DXQUATERNION ret;
    float cx = cos(angle.x/2);
    float sx = sin(angle.x/2);
    float cy = cos(angle.y/2);
    float sy = sin(angle.y/2);
    float cz = cos(angle.z/2);
    float sz = sin(angle.z/2);
    ret.w = cx*cy*cz + sx*sy*sz;
    ret.x = sx*cy*cz - cx*sy*sz;
    ret.y = cx*sy*cz + sx*cy*sz;
    ret.z = cx*cy*sz - sx*sy*cz;
    return ret;
};
//给定角度和轴创建四元数
inline D3DXQUATERNION CQuaternion(D3DVECTOR anxi, const float& angle)
{
    D3DXQUATERNION ret;
    D3DVECTOR t;
    t.x = anxi.x;
    t.y = anxi.y;
    t.z = anxi.z;
    t.normalize();
    float cosa = cos(angle);
    float sina = sin(angle);
    ret.w = cosa;
    ret.x = sina * t.x;
    ret.y = sina * t.y;
    ret.z = sina * t.z;
    return ret;
};
//由旋转四元数推导出矩阵
inline _D3DMATRIX GetMatrixLH(const D3DXQUATERNION &h)
{
    _D3DMATRIX ret;
    float xx = h.x*h.x;
    float yy = h.y*h.y;
    float zz = h.z*h.z;
    float xy = h.x*h.y;
    float wz = h.w*h.z;
    float wy = h.w*h.y;
    float xz = h.x*h.z;
    float yz = h.y*h.z;
    float wx = h.w*h.x;
    ret._11 = 1.0f-2*(yy+zz);
    ret._12 = 2*(xy-wz);
    ret._13 = 2*(wy+xz);
    ret._14 = 0.0f;
    ret._21 = 2*(xy+wz);
    ret._22 = 1.0f-2*(xx+zz);
    ret._23 = 2*(yz-wx);
    ret._24 = 0.0f;
    ret._31 = 2*(xy-wy);
    ret._32 = 2*(yz+wx);
    ret._33 = 1.0f-2*(xx+yy);
    ret._34 = 0.0f;
    ret._41 = 0.0f;
    ret._42 = 0.0f;
    ret._43 = 0.0f;
    ret._44 = 1.0f;
    return ret;
};
inline _D3DMATRIX GetMatrixRH(const D3DXQUATERNION &h)
{
    _D3DMATRIX ret;
    float xx = h.x*h.x;
    float yy = h.y*h.y;
    float zz = h.z*h.z;
    float xy = h.x*h.y;
    float wz = -h.w*h.z;
    float wy = -h.w*h.y;
    float xz = h.x*h.z;
    float yz = h.y*h.z;
    float wx = -h.w*h.x;
    ret._11 = 1.0f-2*(yy+zz);
    ret._12 = 2*(xy-wz);
    ret._13 = 2*(wy+xz);
    ret._14 = 0.0f;
    ret._21 = 2*(xy+wz);
    ret._22 = 1.0f-2*(xx+zz);
    ret._23 = 2*(yz-wx);
    ret._24 = 0.0f;
    ret._31 = 2*(xy-wy);
    ret._32 = 2*(yz+wx);
    ret._33 = 1.0f-2*(xx+yy);
    ret._34 = 0.0f;
    ret._41 = 0.0f;
    ret._42 = 0.0f;
    ret._43 = 0.0f;
    ret._44 = 1.0f;
    return ret;
};
//由四元数返回欧拉角，DX没有提供这个API
inline D3DVECTOR GetEulerAngle(const D3DXQUATERNION &h)
{
    D3DVECTOR ret;
    float test = h.y*h.z + h.x*h.w;
    if (test > 0.4999f)
    {
        ret.z = 2.0f * atan2(h.y, h.w);
        ret.y = 1.570796327f;
        ret.x = 0.0f;
        return ret;
    }
    if (test < -0.4999f)
    {
        ret.z = 2.0f * atan2(h.y, h.w);
        ret.y = -1.570796327f;
        ret.x = 0.0f;
        return ret;
    }
    float sqx = h.x * h.x;
    float sqy = h.y * h.y;
    float sqz = h.z * h.z;
    ret.z = atan2(2.0f * h.z * h.w - 2.0f * h.y * h.x, 1.0f - 2.0f * sqz - 2.0f * sqx);
    ret.y = asin(2.0f * test);
    ret.x = atan2(2.0f * h.y * h.w - 2.0f * h.z * h.x, 1.0f - 2.0f * sqy - 2.0f * sqx);

    return ret;
};
#endif  //_ANGLETRANSFORM_H_
