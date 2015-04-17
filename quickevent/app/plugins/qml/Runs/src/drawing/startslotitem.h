#ifndef DRAWING_STARTSLOTITEM_H
#define DRAWING_STARTSLOTITEM_H

#include <qf/core/utils.h>

#include <QGraphicsRectItem>

namespace drawing {

class ClassItem;
class GanttScene;

class StartSlotData : public QVariantMap
{
public:
	explicit StartSlotData(const QVariantMap &m = QVariantMap()) : QVariantMap(m) {}

	QF_VARIANTMAP_FIELD2(int, s, setS, tart, 0)
	QF_VARIANTMAP_FIELD2(bool, is, set, Locked, false)
};

class StartSlotItem : public QGraphicsRectItem
{
private:
	typedef QGraphicsRectItem Super;
public:
	StartSlotItem(QGraphicsItem * parent = 0);

	QF_FIELD_IMPL2(int, s, S, lotNumber, 0)

public:
	ClassItem* addClassItem();
	int classItemCount();
	ClassItem *classItem(int ix);

	void updateGeometry();
	int posToMin(int pos) const;
	int minToPos(int min) const;
	int pxToMin(int px) const;
	int minToPx(int min) const;

	const StartSlotData& data() const;
	void setData(const StartSlotData &data);
protected:
	int du() const;
private:
	StartSlotData m_data;
	QList<ClassItem*> m_classItems;
	QGraphicsTextItem *m_slotNoText;
};

}

#endif // DRAWING_STARTSLOTITEM_H
