#include "Minimap.h"

Minimap::Minimap(void)
	: m_Minimap_Background(NULL)
	, m_Minimap_Border(NULL)
	, m_Minimap_Avatar(NULL)
	, angle(0)
	, x(0)
	, y(0)
	, size_x(0)
	, size_y(0)
{
}
Minimap::~Minimap(void)
{
}
// Set the background mesh to this class
bool Minimap::SetBackground(Mesh* aBackground)
{
	if (aBackground != NULL)
	{
		m_Minimap_Background = aBackground;
		return true;
	}
	return false;
}
// Get the background mesh to this class
Mesh* Minimap::GetBackground(void)
{
	return m_Minimap_Background;
}
// Set the Border mesh to this class
bool Minimap::SetBorder(Mesh* aBorder)
{
	if (aBorder != NULL)
	{
		m_Minimap_Border = aBorder;
		return true;
	}
	return false;
}
// Get the Border mesh to this class
Mesh* Minimap::GetBorder(void)
{
	return m_Minimap_Border;
}
// Set the Avatar mesh to this class
bool Minimap::SetAvatar(Mesh* anAvatar)
{
	if (anAvatar != NULL)
	{
		m_Minimap_Avatar = anAvatar;
		return true;
	}
	return false;
}
// Get the Avatar mesh to this class
Mesh* Minimap::GetAvatar(void)
{
	return m_Minimap_Avatar;
}
// Set angle of avatar
bool Minimap::SetAngle(const int angle)
{
	this->angle = angle;
	return true;
}
// Get angle
int Minimap::GetAngle(void)
{
	return angle;
}
// Set position of avatar in minimap
bool Minimap::SetPosition(const int x, const int y)
{
	this->x = x;
	this->y = y;
	return true;
}
// Get position x of avatar in minimap
int Minimap::GetPosition_x(void)
{
	return x;
}
// Get position y of avatar in minimap
int Minimap::GetPosition_y(void)
{
	return y;
}
// Set size of minimap (for calculation of avatar in minimap)
bool Minimap::SetSize(const int size_x, const int size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	return true;
}
// Get size of minimap (for calculation of avatar in minimap)
int Minimap::GetSize_x(void)
{
	return size_x;
}
// Get size of minimap (for calculation of avatar in minimap)
int Minimap::GetSize_y(void)
{
	return size_y;
}