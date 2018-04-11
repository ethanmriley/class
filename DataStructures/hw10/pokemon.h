#include <string>
#include <map>
#include <vector>
//pokemon headers for multiple inheritance assignment
//ethan riley, 2017

typedef std::map<std::string, std::vector<std::string> > f_type;

//base class
class Pokemon {

public:
	Pokemon(const f_type& facts_);
	virtual ~Pokemon() {}
	std::string getLabel() const;
	std::vector<std::string> getEggGroups() const;
	bool SharesEggGroup(const Pokemon* p) const;

protected:
	std::map<std::string, std::vector<std::string> > facts;
	std::string color;
	std::string catch_rate;
	std::vector<std::string> egg_groups;
};

//egg group classes
//they're all identical; they just call the Pokemon constructor

class Amorphous : virtual public Pokemon {
public:
	Amorphous(const f_type& facts_) : Pokemon(facts_) {}
};

class Bug : virtual public Pokemon {
public:
	Bug(const f_type& facts_) : Pokemon(facts_) {}
};

class Dragon : virtual public Pokemon {
public:
	Dragon(const f_type& facts_) : Pokemon(facts_) {}
};

class Fairy : virtual public Pokemon {
public:
	Fairy(const f_type& facts_) : Pokemon(facts_) {}
};

class Field : virtual public Pokemon {
public:
	Field(const f_type& facts_) : Pokemon(facts_) {}
};

class Flying : virtual public Pokemon {
public:
	Flying(const f_type& facts_) : Pokemon(facts_) {}
};

class Grass : virtual public Pokemon {
public:
	Grass(const f_type& facts_) : Pokemon(facts_) {}
};

class HumanLike : virtual public Pokemon {
public:
	HumanLike(const f_type& facts_) : Pokemon(facts_) {}
};

class Mineral : virtual public Pokemon {
public:
	Mineral(const f_type& facts_) : Pokemon(facts_) {}
};

class Monster : virtual public Pokemon {
public:
	Monster(const f_type& facts_) : Pokemon(facts_) {}
};

class Water1 : virtual public Pokemon {
public:
	Water1(const f_type& facts_) : Pokemon(facts_) {}
};

class Water2 : virtual public Pokemon {
public:
	Water2(const f_type& facts_) : Pokemon(facts_) {}
};

class Water3 : virtual public Pokemon {
public:
	Water3(const f_type& facts_) : Pokemon(facts_) {}
};

class Ditto: virtual public Pokemon {
public:
	Ditto(const f_type& facts_) : Pokemon(facts_) {}
};

class Undiscovered : virtual public Pokemon {
public:
	Undiscovered(const f_type& facts_) : Pokemon(facts_) {}
};

//specific pokemon class implementation

class NidoranMale : virtual public Pokemon, public Monster, public Field {
public:
	NidoranMale(const f_type& facts_);
};

//pokemon evolutions are both Pokemon and their pre-evolution
class Nidorino : virtual public Pokemon, public NidoranMale {
public:
	Nidorino(const f_type& facts_);
};

class Nidoking : virtual public Pokemon, public Nidorino {
public:
	Nidoking(const f_type& facts_);
};

class Seel : virtual public Pokemon, public Water1, public Field {
public:
	Seel(const f_type& facts_);
};

class Dewgong : virtual public Pokemon, public Seel {
public:
	Dewgong(const f_type& facts_);
};

class Shellos : virtual public Pokemon, public Water1, public Amorphous {
public:
	Shellos(const f_type& facts_);
};

class Gastrodon : virtual public Pokemon, public Shellos {
public:
	Gastrodon(const f_type& facts_);
};

class Finneon : virtual public Pokemon, public Water2 {
public:
	Finneon(const f_type& facts_);
};

class Lumineon : virtual public Pokemon, public Finneon {
public:
	Lumineon(const f_type& facts_);
};

class Basculin : virtual public Pokemon, public Water2 {
public:
	Basculin(const f_type& facts_);
};