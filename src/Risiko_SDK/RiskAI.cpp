#include "RiskAI.h"

RiskAI::RiskAI()
	: QObject()
{	

}

RiskAI::~RiskAI()
{

}

void RiskAI::initAI( Player *player, RiskMap *map, RiskGameLogic *logic )
{
	this->player = player;
	this->map = map;
	this->logic = logic;
}
