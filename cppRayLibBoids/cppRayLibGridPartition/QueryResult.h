#ifndef QUERYRESULT_H
#define QUERYRESULT_H

class Particle;

struct QueryResult
{
	Particle* center{ nullptr };
	
	Particle* topLeft{ nullptr };
	Particle* topMiddle{ nullptr };
	Particle* topRight{ nullptr };

	Particle* centerLeft{ nullptr };
	Particle* centerRight{ nullptr };

	Particle* bottomLeft{ nullptr };
	Particle* bottomMiddle{ nullptr };
	Particle* bottomRight{ nullptr };

};

#endif