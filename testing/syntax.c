/*
 * Define:
 *	ERR_YACC	to force yacc error reporting
 *	ERR_LEX		to force lex error reporting
 *	ERR_CHECK	to force compiler error reporting
 */
		/* use one item from type-specifiers */
#ifdef	ERR_YACC
auto		x;
register	x1;
#endif	ERR_YACC
static		x2;
extern		x3;
#ifdef	ERR_CC
typedef		x4;
#endif
typedef	int	x4t;
#ifdef	ERR_YACC
void		x5;
#endif	ERR_YACC
char		x6;
short		x7;
int		x8;
long		x9;
float		x10;
double		x11;
signed		x12;
unsigned	x13;
struct		x14;
#ifdef	ERR_CHECK
struct		x14a {};
struct		{};
#endif	ERR_CHECK
union		x15;
enum		x16;
x4t;
x4t		x17;
const		x18;
volatile	x19;
#ifdef	ERR_CHECK
junk		x20;
#endif	ERR_CHECK

typedef	struct	_first	{
	int		a:5;
	struct	{
		int	a,b;
	} b_struct;
	char		b:16, b1:1;	/* comment with };};}; */
	long		c:16, c1;
	short		d:16, d1:8, d2;
	unsigned	e:16;
	float		f;
	double		g;
#ifdef	ERR_CHECK
	long	float	f2;
	long	double	g2;
#endif	ERR_CHECK
	struct	_first	*link;
	}	_FIRST;

_FIRST	first, last={0}, first_last[] = {{0},{1},{2},{3,{4}}};

struct	_second	{
		enum	{true, false} bool;
		enum	{wrong=1, right=3} values;
	} _SECOND;

int	i[] = {1,
		'\002',
		03,
		0x4,
		0X5,
		0x6a,
		0X7b,
		0x8aBcD,
		9l,
		10l,
		11L};
float	f[] = {5,
#ifdef	ERR_CHECK
		.5,
		.5e,
		.5e+,
		5e-,
		5e,
#endif
		5.5,
		5e5,
		5e0,
		5e-1,
		5e+5
		};

int	array[][10][20];

	/*
	 * This grammar can accept some illegal stuff too, in particular it will
	 * permit some instances of parameter-names to be dropped.
	 */
#ifdef	ERR_CHECK
#define	P1
#define	P2
#define	P3
#else
#define	P1	p1
#define	P2	p2
#define	P3	p3
#endif

	/* ellipsis is legal, except in K&R style declaration */

	dots_0(p1)		{ return(1); }

	dots_1(int p1, ...)	{ return(1); }

#ifdef	ERR_CHECK
	dots_2(p1, ...)		{ return(1); }
#endif	ERR_CHECK

	dots_3(int P1,char P2, ...)	{ return(1); }

int	dots_4(int,char, char *);
char *	dots_5(int,char, char *, ...);

extern	_FIRST *xdots_0(p1);

extern	char *xdots_1(int p1, ...);

#ifdef	ERR_CHECK
extern	xdots_2(p1, ...);
#endif	ERR_CHECK

extern	xdots_3(int P1,char P2, ...);

_FIRST	*func1() { }
_FIRST	*func2(_FIRST *P1,int P2[],float p) { }

int	*(func_func)(
#ifdef	ERR_CHECK
p1,p2,p3
#endif	ERR_CHECK
)	{ return(0); }

extern	float	efunc0(p1,p2,p3);
extern	_FIRST	efunc1(int p1, float p2,long P3);
#ifdef	ERR_CHECK
extern	_FIRST	efunc1(int p1, float p2,p3);
#endif	ERR_CHECK


typedef	int	bool;

bool	a1;
static	bool	a2;
extern	bool	a3;

struct	ZIP1	{ int x1, y1; };
struct	zip2	{ int x2, y2; } z2;
struct		{ int x3, y3; } z3;	/* not ANSI, but mostly accepted */

static	struct	ZIP4	{ int x4, y4; };
static	struct	zip5	{ int x5, y5; } z5;
static	struct	zip6	{ int x6, y6; } z6, w6;
static	struct		{ int x7, y7; } z7;
static	struct		{ int x8, y8; } z8, w8;

enum	zap1	{ a,b,c,d }	what;
enum	zap2			what2;
enum		{a9,b9,c9,d9}	what3;

static	char	*zap = "alphabet/\
first/\
last";
typedef	struct	bong	{
	int	(*func)();
	} BONG;

typedef	char	*string;

#ifdef	ERR_LEX
string	s = "aaa\0000\

bbb";
#endif	ERR_LEX

extern	int	junk;

main(argc, argv)
register argc;
#ifdef	ERR_CHECK
extern
#endif	ERR_CHECK
char	**argv;
{
}

/*VARARGS*/
/*VARARGS3*/
program(argv, y, zz, z1, z2, z3)
string	argv[];	/* first argument */
struct	alpha { int x,z; } y;	/* second argument */
{
	return(0);
}

junk0() { if(junk != 6) return; else junk++; }
junk1() { return (0); }
junk2() { }
junk3() { return 1; }

BONG	*junk4() { }

typedef	int	extern_junk;
extern_junk	*foo() { }

typedef	int	void_junk;
extern void_junk	*foo2a() { }
extern void_junk	*foo2a();
void_junk	*foo2() { }
void_junk	*foo_void(void_junk void_int) { }
static	void_junk	*foo_void2() { }

extern void (*sigdisp(int sig, void (*func)(int sig)))(int sig);

void (*Sigdisp(int sig, void (*func)(int sig)))(int sig)
{ /* nothing */ }

void (*sigdisp2(sig, func))(int sig)
	int sig;		/* the signal value */
	void (*func)(int sig);	/* the function pointer */
{ /* nothing again! */ }
