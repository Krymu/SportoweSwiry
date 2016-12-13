#include "SHIP.H"
#include <cmath>



bool Ship::CanShoot() {
	if (Lives > 0 && RemainingShoots > 0)
		return true;
	else
		return false;

}

bool Ship::isTargetInRange(float _length, coords target ) {

	float range = _length+1;

	if (abs(target.x - this->x_begin) < range || abs(target.x - this->x_end) < range)
	{
		if (abs(target.y - this->y_begin) < range || abs(target.y - this->y_end) < range)
			return true;
	}
	else
		return false;

} //EEEE to chyba tak moze byc ? xd

void Ship::Shot() {
	//pozniej xd
}

void Ship::getHit() {
	this->Lives--;
	
	if (Lives == 0)
		this->Sink();
}

void Ship::Sink() {
	//to graphical UI 
}

bool Ship::isSunk(){

	if (!this->Lives)
		return true;
	else 
		return false;
}

int Ship::getType() {
	return this->Lenght;
}

void Ship::setCoords(coords start, coords end) {
	this->x_begin = start.x;
	this->x_end = end.x;
	this->y_begin = start.y;
	this->y_end = end.y;
}

SingleFunnelShip::SingleFunnelShip( Grid* p_grid,coords coordinates) {
	
	this->grid = p_grid;
	this->Lives = 1;
	this->Lenght = 1;
	this->setCoords(coordinates,coordinates);
	this->RemainingShoots = 1;
}
void SingleFunnelShip::Reset() {
	this->RemainingShoots = 1;
}

MultiFunnelShip::MultiFunnelShip(int ship_type, Grid* p_grid, coords coordinates_start, coords coordinates_end) {
	this->grid = p_grid;
	this->Lives = 1;
	this->Lenght = 1;
	this->setCoords(coordinates_start, coordinates_end);
	this->RemainingShoots = ship_type - 1;
	this->ShotTwice = false;
}

void MultiFunnelShip::Reset() {
	if (ShotTwice)
		RemainingShoots = 1;
	else
		RemainingShoots = 2;
	
	ShotTwice = false;
}