
#ifndef _AGLIB_H_INCLUDED_
#define _AGLIB_H_INCLUDED_

/* return value */
#define AG_ERROR_OK			0
#define AG_ERROR_OUT_OF_MEM 1
#define EXPAND_NO			0
#define EXPAND_1_LEFT		1
#define EXPAND_1_RIGHT		2
#define EXPAND_2_LEFT		4
#define EXPAND_2_RIGHT		8
#define EXPAND_ALL			15

#define BE_BASE(x0, x1, x2, x3, t)	(3*(x1)-(x0)-3*(x2)+(x3))*(t)*(t)*(t)\
	+(3*(x0)+3*(x2)-6*(x1))*(t)*(t)\
	+(-3*(x0)+3*x1)*(t) +(x0)

typedef struct tagVECTOR{
	double x;
	double y;
	double z;
}VECTOR;

typedef struct tagVECTOR2D{
	double x;
	double y;
}VECTOR2D;

typedef struct tagBSPLINE{
	VECTOR	*pCtrlPoints;
	double	*pKnots;
	int		nPoints;
	int		nDegree;
}BSPLINE;

typedef struct tagBSPLINESURFACE{
	VECTOR	*pCtrlPoints;
	double	*pKnotsu;
	double	*pKnotsv;
	int		nPointsu;
	int		nPointsv;
	int		nDegreeu;
	int		nDegreev;
}BSPLINESURFACE;

typedef struct tagLINE{
	VECTOR v;
	struct tagLINE *pNext, *pPre;
}LINE;

typedef struct tagINTERSECTION_LINES{
	LINE	*pLineHead;
	LINE	*pLineTail;
	int		nPointsCounter;
	struct tagINTERSECTION_LINES *pNext;
}INTERSECTION_LINES;

typedef struct tagFACE{
	VECTOR avPoints[3];
	VECTOR vNormal;
}FACE;

typedef struct tagFACE_LIST{
	FACE face;
	struct tagFACE_LIST *pNext;
}FACE_LIST;

typedef struct tagCONVEX_HULL{
	FACE_LIST faceList;
	int	nFaceNumber;
}CONVEX_HULL;

#ifndef MAX
#define MAX(x, y)	(x)>(y)?(x):(y)
#endif

#ifndef MIN
#define MIN(x, y)	(x)<(y)?(x):(y)
#endif

//#define SIGN(x) ((x)>0?1:((x)<0?-1:0))
#define SIGN(x) ((x)>=0?1:-1)

#define DE_BOOR(bs, u, pvPoints) DeBoor((bs)->pCtrlPoints, (bs)->pKnots, (bs)->nPoints, (bs)->nDegree, (u), (pvPoints))

#define DISTANCE(v1, v2) sqrt(((v1).x-(v2).x)*((v1).x-(v2).x) + \
	((v1).y-(v2).y)*((v1).y-(v2).y) + \
	((v1).z-(v2).z)*((v1).z-(v2).z))

/* functions */
int Bezier2D(VECTOR2D *aControlVector, VECTOR2D *v, double t);
int Bezier(VECTOR *aControlVector, VECTOR *v, double t);
int DeCasteljau(VECTOR avCtrlPoint[], int n, double t, VECTOR *pvPoint);

int Intersection(VECTOR avCtrlPoint1[], int nPoints1, VECTOR avCtrlPoint2[], int nPoints2,
				 VECTOR aVector[], int nSize,	/* double aT1[], double aT2[] */
				 int nExpand, double t1l, double t1r, double t2l, double t2r);
int Intersection2D(VECTOR2D v1[], VECTOR2D v2[], VECTOR2D aPoint[], int nSize);
int SectionSplit2D(VECTOR2D v[], double aT[]);
int SetPrecision(double epsilon);
double GetPrecision();
void BezierIncreaseOrder(VECTOR *pvOld, VECTOR *pvNew, int n);
int BezierTangent(VECTOR avCtrlPoint[], int n, double t, VECTOR *pvPoint);
int BezierTangent2(VECTOR avCtrlPoint[], int n, double t, VECTOR *pvPoint);
int SectionSplit(VECTOR vCtrl[], int n, double aT[]);
int ExpandBezierLeft(VECTOR avCtrlPoint[], int nPoints, double fLeft);
int ExpandBezierRight(VECTOR avCtrlPoint[], int nPoints, double fRight);
int DeBoor(VECTOR avCtrlPoints[], double aKnots[], int nPoints, int nDegree,
		   double u, VECTOR* pvPoint);
int SplitB_Spline(BSPLINE *pCurve, double u, BSPLINE *pCurveLeft, BSPLINE *pCurveRight);
int IntersectionLine(BSPLINESURFACE* , BSPLINESURFACE* , INTERSECTION_LINES  *pIntLines);
int BuildConvexHull(BSPLINESURFACE *pbfSurface, CONVEX_HULL *pchHull);
int IsConvexHullIntersect(CONVEX_HULL *pConvexHull0, BSPLINESURFACE *pSurface0,
						  CONVEX_HULL *pConvexHull1, BSPLINESURFACE *pSurface1);

int IsTriangleIntersect(FACE *pFace0, FACE *pFace1, VECTOR * pV);
int SplitBSplineSurface(BSPLINESURFACE *pbfSurface, BSPLINESURFACE abfPatch[4]);
void CollectLines(INTERSECTION_LINES * pLineSet0, INTERSECTION_LINES * pLineSet1);

/* Invalidity checking functions.*/
int CheckKnots(BSPLINESURFACE * pbfSurface);
void CheckKnots(BSPLINE *pbs);
void CheckCtrlPoints(BSPLINESURFACE *pbf);
void CheckConvexHull(CONVEX_HULL *pConvexHull);
void CheckIntersectionLines(INTERSECTION_LINES *pIntLines);
void CheckConvexTriangles(CONVEX_HULL *pConvexHull);
void CheckPoints(INTERSECTION_LINES *pIntLine0, INTERSECTION_LINES *pIntLine1);

/* Destruction functions.*/
void DestructIntersectionLines(INTERSECTION_LINES *pIntLines);
void DestructBsplineCurve(BSPLINE *pBspline);
void DestructBSplineSurface(BSPLINESURFACE *pbfSurface);
void DestructConvexHull(CONVEX_HULL * pConvexHull);

#define NEWTON_ROOT(SUB, avCtrl, n, t0, t1, pT)   \
	double x0, x1;\
	int nTime=0;\
	VECTOR vPoint1, vPoint2;\
\
	x0 = t0;\
	BezierTangent(avCtrl, n, x0, &vPoint1);\
	BezierTangent2(avCtrl, n, x0, &vPoint2);\
	x1 = x0 - vPoint1.##SUB/vPoint2.##SUB;\
	while(fabs(x1 - x0)>g_epsilon*0.01 && nTime<50)\
	{\
		BezierTangent(avCtrl, n, x0, &vPoint1);\
		BezierTangent2(avCtrl, n, x0, &vPoint2);\
		x0 = x1;\
		x1 = x0 - vPoint1.##SUB/vPoint2.##SUB;\
		nTime++;\
	}\
	if(x1>=t0 && x1<= t1 && nTime<50)\
	{\
		*pT = x1;\
		return 1;\
	}\
	else\
		return 0;\


#endif /* _AGLIB_H_INCLUDED_ */