constexpr auto PI = 245850922.0/78256779.0; //PI 3.14159265358979323846264338327950288419716939937510
constexpr auto G = 6.674e-11;				//GravitationalConstant			N·kg–2·m2
constexpr auto c = 2.99792458e8;			//speed of Light in Vaccum		m/s
constexpr auto h = 6.626070040e-34;			//Plank'sConstant				J·s
constexpr auto hbar = h / 2 * PI;			//Reduced Planck constant.		J·s
constexpr auto eV = 1.6021766208e-19;		//Unit of energy  				J		
constexpr auto eVc2 = eV / (c*c);			//Unit of Mass					Kg
constexpr auto eVc = eV / (c);				//Unit of Momentum				Kg·m/s


struct fermion {
	float spin = 1/2;	
	float charge;		
	float mass;		//MeV/c²
	float antip = 1;
};

//QUARKS

class up_quark : public fermion{
public:
	up_quark() {
		spin = 1 / 2;
		charge = 2 / 3;
		mass = 2.2; //+0.6 -0.4
		antip = 1; 
	}
};

class charm_quark : public fermion {
public:
	charm_quark() {
		spin = 1 / 2;
		charge = 2 / 3;
		mass = 1280; //+-30
		antip = 1;
	}
};

class top_quark : public fermion {
public:
	top_quark() {
		spin = 1 / 2;
		charge = 2 / 3;
		mass = 173100; //+-600
		antip = 1;
	}
};

class down_quark : public fermion {
public:
	down_quark() {
		spin = 1 / 2;
		charge = -1 / 3;
		mass = 4.6;  //+0.5 -0.4;
		antip = 1;
	}
};

class strange_quark : public fermion {
public:
	strange_quark() {
		spin = 1 / 2;
		charge = -1 /3;
		mass = 96; //+8 -4
		antip = 1;
	}
};

class bottom_quark : public fermion {
public:
	bottom_quark() {
		spin = 1 / 2;
		charge = -1 / 3;
		mass = 4180; //+40 -30
		antip = 1;
	}
};

//LEPTONS

//class electron : public fermion {
//public:
//	Electron() {
//		spin = 1 / 2;
//		charge = -1 / 3;
//		mass = 4180; //+40 -30
//		antip = 1;
//	}
//};







