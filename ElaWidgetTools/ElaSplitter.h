#ifndef ELASPLITTER_H
#define ELASPLITTER_H

#include <QSplitter>

#include "ElaProperty.h"

class ElaSplitterPrivate;
class ELA_EXPORT ElaSplitter : public QSplitter
{
	Q_OBJECT
	Q_Q_CREATE(ElaSplitter)
	Q_PROPERTY_CREATE_Q_H(int, HandleWidth)
	Q_PROPERTY_CREATE_Q_H(int, GripLength)
public:
	explicit ElaSplitter(QWidget* parent = nullptr);
	explicit ElaSplitter(Qt::Orientation orientation, QWidget* parent = nullptr);
	~ElaSplitter() override;

protected:
	QSplitterHandle* createHandle() override;
};

#endif // ELASPLITTER_H
