#include "stdafx.h"
#include "CurrentSource.h"


CurrentSource::CurrentSource()
{
}


CurrentSource::~CurrentSource()
{
}

void CurrentSource::addStamp(double ** Matrix, unsigned variables)
{
	if(nodes.at(0) != 0)
		Matrix[nodes.at(0) - 1][variables] -= Value;
	if(nodes.at(1) != 0)
		Matrix[nodes.at(1) - 1][variables] += Value;
}
