#include "vector2D.h"

Vector2D::Vector2D() {
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::Add(const Vector2D& vec) {
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vec) {
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D& Vector2D::Multipliy(const Vector2D& vec) {
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vec) {
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D& operator +(Vector2D& v1, const Vector2D& v2) {
	return v1.Add(v2);
}

Vector2D& operator -(Vector2D& v1, const Vector2D& v2) {
	return v1.Subtract(v2);
}

Vector2D& operator *(Vector2D& v1, const Vector2D& v2) {
	return v1.Multipliy(v2);
}

Vector2D& operator /(Vector2D& v1, const Vector2D& v2) {
	return v1.Divide(v2);
}

Vector2D& Vector2D::operator +=(const Vector2D& vec) {
	return this->Add(vec);
}

Vector2D& Vector2D::operator -=(const Vector2D& vec) {
	return this->Subtract(vec);
}

Vector2D& Vector2D::operator *=(const Vector2D& vec) {
	return this->Multipliy(vec);
}

Vector2D& Vector2D::operator /=(const Vector2D& vec) {
	return this->Divide(vec);
}

Vector2D& Vector2D::operator *(const int& i) {
	this->x *= i;
	this->y *= y;
	return *this;
}

Vector2D& Vector2D::Zero() {
	this->x = 0.0f;
	this->y = 0.0f;

	return *this;
}

std::ostream& operator<<(std::ostream stream, const Vector2D vec) {
	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
}