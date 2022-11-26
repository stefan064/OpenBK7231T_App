#ifdef WINDOWS
#include "Shape.h"
#include "Circle.h"
#include "Rect.h"
#include "Line.h"
#include "Text.h"
#include "Junction.h"
#include "Controller_Base.h"
#include "Controller_Button.h"

CShape::~CShape() {
	for (int i = 0; i < shapes.size(); i++) {
		delete shapes[i];
	}
	shapes.clear();
	if (controller != 0) {
		delete controller;
		controller = 0;
	}
}

float CShape::drawPrivateInformation2D(float x, float h) {
	h = drawText(x, h, "Position: %f %f", this->getX(), getY());
	return h;
}
float CShape::drawInformation2D(float x, float h) {
	h = drawText(x, h, "ClassName: %s", this->getClassName());
	h = drawPrivateInformation2D(x, h);
	if (shapes.size()) {
		h = drawText(x, h, "SubShapes: %i", shapes.size());
		for (int i = 0; i < shapes.size(); i++) {
			h = drawText(x + 20, h, "SubShape: %i/%i", i, shapes.size());
			h = shapes[i]->drawInformation2D(x + 40, h);
		}
	}
	return h;
}

void CShape::translateEachChild(float oX, float oY) {
	for (int i = 0; i < shapes.size(); i++) {
		shapes[i]->translate(oX, oY);
	}
}
bool CShape::hasWorldPointInside(const Coord &p)const {
	Coord loc = p - this->getPosition();
	return hasLocalPointInside(loc);
}
bool CShape::hasLocalPointInside(const Coord &p)const {
	return bounds.isInside(p);
}
void CShape::rotateDegreesAround(float f, const Coord &p) {
	Coord p_i = p - this->getPosition();
	rotateDegreesAround_internal(f, p);
	if (controller != 0) {
		controller->rotateDegreesAround(f, p_i);
	}
	for (int i = 0; i < shapes.size(); i++) {
		shapes[i]->rotateDegreesAround(f, p_i);
	}
	recalcBoundsAll();
}
void CShape::rotateDegreesAroundSelf(float f) {
	//rotateDegreesAround(f, Coord(0, 0));
	rotateDegreesAround(f, getPosition());
}
void CShape::recalcBoundsAll() {
	bounds.clear();
	this->recalcBoundsSelf();
	for (int i = 0; i < shapes.size(); i++) {
		shapes[i]->recalcBoundsAll();
		bounds.addBounds(shapes[i]->getBounds(), shapes[i]->getPosition());
	}
}
class CShape* CShape::addLine(int x, int y, int x2, int y2) {
	CLine *n = new CLine(x, y, x2, y2);
	addShape(n);
	return n;
}
class CShape* CShape::addShape(CShape *p) {
	p->parent = this;
	shapes.push_back(p);
	return p;
}
class CShape* CShape::addJunction(int x, int y, const char *name) {
	CJunction *n = new CJunction(x, y, name);
	addShape(n);
	return n;
}
class CShape* CShape::addCircle(float x, float y, float r) {
	CCircle *n = new CCircle(x, y, r);
	addShape(n);
	return n;
}
class CShape* CShape::addRect(int x, int y, int w, int h) {
	CRect *n = new CRect(x, y, w, h);
	addShape(n);
	return n;
}
class CShape* CShape::addText(int x, int y, const char *s) {
	CText *n = new CText(x, y, s);
	addShape(n);
	return n;
}
Coord CShape::getAbsPosition() const {
	Coord ofs = this->getPosition();
	const CShape *p = this->getParent();
	while (p) {
		ofs += p->getPosition();
		p = p->getParent();
	}
	return ofs;
}
void CShape::translate(const Coord &ofs) {
	pos += ofs;
	// special handling for connected wires
	for (int i = 0; i < shapes.size(); i++) {
		CShape *o = shapes[i];
		if (o->isJunction() == false)
			continue;
		CJunction *j = dynamic_cast<CJunction*>(o);
		j->translateLinked(ofs);
	}
}

void CShape::rotateDegreesAround_internal(float f, const Coord &p) {
	pos = pos.rotateDegreesAround(f, p);
}



void CShape::moveTowards(const Coord &tg, float dt) {
	pos = pos.moveTowards(tg, dt);
}
void CShape::drawWithChildren(int depth) {
	if (controller != 0) {
		controller->onDrawn();
	}
	drawShape();
	glPushMatrix();
	glTranslatef(getX(), getY(), 0);
	for (int i = 0; i < shapes.size(); i++) {
		shapes[i]->drawWithChildren(depth + 1);
	}
	//recalcBoundsAll();
	if (depth == 0) {
		glColor3f(0, 1, 0);
		glLineWidth(0.5f);
		glBegin(GL_LINE_LOOP);
		Bounds bb = bounds;
		bb.extend(4);
		for (int i = 0; i < 4; i++) {
			glVertex2fv(bb.getCorner(i));
		}
		glEnd();
	}
	glPopMatrix();
}



#endif
