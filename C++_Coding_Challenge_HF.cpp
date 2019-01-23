//Badr Mejri Source Code, 2019 for Hidden Founders.

#include <iostream>

using namespace std;
// =====================================================
//   Analyser et tester les Expressions arithmétiques
// =====================================================

// Les différents types d'erreurs prédit
enum type_Err
{
	no_Err = 0,
	parenthesis_Err = 1,
	char_Err = 2,
	illegal_Div = 3
};

typedef char Test_Char;

class Trait_Expr
{
private:
	type_Err Err;
	Test_Char *pos_Err;
	int nbre_parenth;

	// Analyser une Expression qui existe dans des parenthèses
	double EvalAtom(Test_Char*& Expr)
	{
		// Sauter les espaces
		while(*Expr == ' ')
			Expr++;

		// Manipuler les signes
		bool negative = false;
		if(*Expr == '-')
		{
			negative = true;
			Expr++;
		}
		if(*Expr == '+')
		{
			Expr++;
		}

		// Vérifier l'existence des parenthèses
		if(*Expr == '(')
		{
			Expr++;
			nbre_parenth++;
			double Result = EvalSummands(Expr);
			if(*Expr != ')')
			{
				// Parenthèse non fermé
				Err = parenthesis_Err;
				pos_Err = Expr;
				return 0;
			}
			Expr++;
			nbre_parenth--;
			return negative ? -Result : Result;
		}

		// Convertir l'Expression en double
		char* Expr_fin;
		double Result = strtod(Expr, &Expr_fin);
		if(Expr_fin == Expr)
		{
			// Report Error
			Err = char_Err;
			pos_Err = Expr;
			return 0;
		}
		// Avancer le pointeur retourner le résultat
		Expr = Expr_fin;
		return negative ? -Result : Result;
	}

	// Analyse de la division et la multiplication
	double EvalFactors(Test_Char*& Expr)
	{
		double fNumber = EvalAtom(Expr);
		while(1)
		{
			// Sauter les espaces
			while(*Expr == ' ')
				Expr++;
			// Sauvegarder l'opération et la position
			Test_Char op = *Expr;
			Test_Char* pos = Expr;
			if(op != '/' && op != '*')
				return fNumber;
			Expr++;
			double secNumber = EvalAtom(Expr);
			// Traiter l'opération enregistrée
			if(op == '/')
			{
				// Erreur division par zéro
				if(secNumber == 0)
				{
					Err = illegal_Div;
					pos_Err = pos;
					return 0;
				}
				fNumber /= secNumber;
			}
			else
				fNumber *= secNumber;
		}
	}

	// Analyser l'addition et la soustraction
	double EvalSummands(Test_Char*& Expr)
	{
		double fNumber = EvalFactors(Expr);
		while(1)
		{
			// Sauter les espaces
			while(*Expr == ' ')
				Expr++;
			Test_Char op = *Expr;
			if(op != '-' && op != '+')
				return fNumber;
			Expr++;
			double secNumber = EvalFactors(Expr);
			if(op == '-')
				fNumber -= secNumber;
			else
				fNumber += secNumber;
		}
	}

public:
	double Eval(Test_Char* Expr)
	{
		nbre_parenth = 0;
		Err = no_Err;
		double Result = EvalSummands(Expr);
		// l'Expression devrait pointer sur '\ 0' et nbre_parenth devrait être égal à zéro
		if(nbre_parenth != 0 || *Expr == ')')
		{
			Err = parenthesis_Err;
			pos_Err = Expr;
			return 0;
		}
		if(*Expr != '\0')
		{
			Err = char_Err;
			pos_Err = Expr;
			return 0;
		}
		return Result;
	};
	type_Err getErr()
	{
		return Err;
	}
	Test_Char* getErrPos()
	{
		return pos_Err;
	}
};


//==============
// Programme Principale
//==============

int main()
{

	static const char *Errors[] = {
		"Pas d'erreurs",
		"Erreur dans les parenthèses",
		"Charactère non-valide",
		"Division par zéro"};

	puts("Enter une expression (ou une chaine vide pour quitter):");
	while(1)
	{
		// Obtenir une chaîne à partir de la console
		static char ch[256];
		char *Expr = gets(ch);

		// Si la chaîne est vide, quitter l'exécution
		if(*Expr == '\0')
			return 0;

		// Tester l'expression
		Trait_Expr tExpression;
		double dResult = tExpression.Eval(Expr);
		if(tExpression.getErr() != no_Err)
		{
			printf("  Erreur: %s at %s\n", Errors[tExpression.getErr()], tExpression.getErrPos());
		}
		else
		{
			cout<<"Le résultat de l'expression saisie est  "<< dResult << endl;
		}
	}
}
