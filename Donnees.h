#if ! defined DONNEES_H
#define DONNEES_H
#define NB_PLACES 8

struct StructEtat {
	long type; 
	int place;
};
typedef struct StructEtat StructEtat;

struct StructRequete {
	TypeBarriere typeBarriere; 
	TypeUsager usager;
};
typedef struct StructRequete StructRequete;
#endif
