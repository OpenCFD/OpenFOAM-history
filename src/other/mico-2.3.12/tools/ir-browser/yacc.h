typedef union {
	int		    val;
	string             *str;
	SetOfString        *strs;
	Node		   *node;
	ConceptualGraph	   *cg;
} YYSTYPE;
#define	INVOKE	257
#define	BIND	258
#define	RELEASE	259
#define	ERROR	260
#define	ANSWER_GRAPH	261
#define	ANSWER_KEY	262
#define	ANSWER_OK	263
#define	TOKEN_UNDEF	264
#define	CONCEPT_OPEN	265
#define	CONCEPT_CLOSE	266
#define	COLON	267
#define	RELATION_OPEN	268
#define	RELATION_CLOSE	269
#define	CONNECTS	270
#define	COMMA	271
#define	SUBGRAPH_OPEN	272
#define	SUBGRAPH_CLOSE	273
#define	EQUAL	274
#define	BRACKET_OPEN	275
#define	BRACKET_CLOSE	276
#define	SEMICOLON	277
#define	STRING	278
#define	VALUE	279


extern YYSTYPE yylval;
