#ifndef ELASPLITTERPRIVATE_H
#define ELASPLITTERPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class ElaSplitter;
class ElaSplitterPrivate : public QObject
{
	Q_OBJECT
	Q_D_CREATE(ElaSplitter)
	Q_PROPERTY_CREATE_D(int, HandleWidth)
	Q_PROPERTY_CREATE_D(int, GripLength)
public:
	explicit ElaSplitterPrivate(QObject* parent = nullptr);
	~ElaSplitterPrivate() override;
};

#endif // ELASPLITTERPRIVATE_H
