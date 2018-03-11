
//
// Author: Frantisek Vacek <fanda.vacek@volny.cz>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef QF_QMLWIDGETS_REPORTS_REPORTPROCESSOR_H
#define QF_QMLWIDGETS_REPORTS_REPORTPROCESSOR_H

#include "../../qmlwidgetsglobal.h"
#include "reportitem.h"
#include "reportdocument.h"
//#include "reportprocessorcontext.h"

#include <qf/core/utils/treetable.h>
//#include <qf/core/utils/searchdirs.h>
#include <qf/core/assert.h>

#include <QObject>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QPointer>

class QPrinter;
class QQmlEngine;

namespace qf {
namespace qmlwidgets {
namespace reports {

class ReportItem;
class ReportItemBand;
class ReportItemMetaPaint;
class ReportItemMetaPaintReport;


class  QFQMLWIDGETS_DECL_EXPORT ReportProcessor : public QObject
{
	Q_OBJECT
	//! cislo stranky, ktera se zrovna zpracovava, pocitaji se od 0.
	Q_PROPERTY(int processedPageNo READ processedPageNo WRITE setProcessedPageNo NOTIFY processedPageNoChanged)
public:
	class QFQMLWIDGETS_DECL_EXPORT HtmlExportOptions : public QVariantMap
	{
	public:
		HtmlExportOptions(const QVariantMap &o = QVariantMap()) : QVariantMap(o) {}

		QF_VARIANTMAP_FIELD2(bool, isC, setC, onvertBandsToTables, true)
	};
public:
	enum ProcessorMode {SinglePage = 1, FirstPage, AllPages};
	typedef QMap<QString, ReportItem::Image> ImageMap;
public:
	ReportProcessor(QPaintDevice *paint_device, QObject *parent = NULL);
	~ReportProcessor() Q_DECL_OVERRIDE;

	QF_PROPERTY_IMPL(int, p, P, rocessedPageNo)
public:
	/// vymaze vsechna data vznikla predchozimi kompilacemi
	void reset();
	//--void setReport(const ReportDocument &doc);
	bool setReport(const QString &rep_file_name, const QVariantMap &report_init_properties = QVariantMap());
	QUrl reportUrl() const;
	//--ReportDocument report() {return fReport;}
	//--ReportDocument& reportRef() {return fReport;}
	void setTableData(const QString &key, const QVariant &table_data);
	void setTableData(const QString &key, const qf::core::utils::TreeTable &table_data);
	void setData(const QString &key, const QVariant& data);
	QVariant data(const QString &key) const {return m_data.value(key);}

	void addImage(const QString key, const ReportItem::Image &img) {m_imageMap[key] = img;}
	const ImageMap& images() const {return m_imageMap;}

	bool isDesignMode() const {return m_designMode;}
	void setDesignMode(bool b) {m_designMode = b;}
public:
	void setPaintDevice(QPaintDevice *pd) {m_paintDevice = pd;}
	QPaintDevice* paintDevice() {
		QF_ASSERT_EX(m_paintDevice, "paintDevice cannot be null");
		return m_paintDevice;
	}
	//! Vrati QFontMetricsF pro \a font a \a paintDevice() .
	//! Pokud je paintDevice NULL, vrati fontMetrics pro screen.
	QFontMetricsF fontMetrics(const QFont &font);
protected:
	virtual ReportItem::PrintResult processPage(ReportItemMetaPaint *out);
	/// return NULL if such a page does not exist.
	ReportItemMetaPaintFrame *getPage(int page_no);
public:
	virtual void process(ProcessorMode mode = AllPages);
	void print(QPrinter &printer, const QVariantMap &options);

	int pageCount();

	ReportItemMetaPaintReport* processorOutput() {return m_processorOutput;}
	ReportItemReport* documentInstanceRoot();
public:
	/// vlozi do el_body report ve formatu HTML
	virtual void processHtml(QDomElement &el_body, const HtmlExportOptions &opts = HtmlExportOptions());
	static QString HTML_ATTRIBUTE_ITEM;
	static QString HTML_ATTRIBUTE_LAYOUT;

	void dump();
protected:
	//void fixTableTags(QDomElement &el);
	QDomElement removeRedundantDivs(QDomElement &el);
	QDomElement convertBandsToTables(QDomElement &el);
	QDomElement convertHorizontalDivToTableRow(QDomElement &el_div);
signals:
	//! emitovan vzdy, kdyz procesor dokonci dalsi stranku.
	void pageProcessed();
public slots:
	//! prelozi dalsi stranku reportu (takhle delam multithreading, protoze QFont musi bezet v GUI threadu)
	void processSinglePage() {process(SinglePage);}
public:
	/// Every QnlEngine created by ReportProcessor will have this import paths
	static QStringList& qmlEngineImportPaths();
protected:
	QQmlEngine* qmlEngine(bool throw_exc = true);
private:	
	ImageMap m_imageMap;

	QQmlEngine *m_qmlEngine = nullptr;
	ReportDocument *m_reportDocumentComponent = nullptr;
	QVariantMap m_reportInitProperties;
	QVariantMap m_data;
	QPaintDevice *m_paintDevice = nullptr;
	//! Tree of instantiated QML report objects.
	ReportItemReport *m_documentInstanceRoot = nullptr;
	//! Root of ReportItemMetaPaint objects tree generated by \a printMetaPaint() method.
	ReportItemMetaPaintReport *m_processorOutput = nullptr;

	ReportItem::PrintResult m_singlePageProcessResult;

	//! designated for QML Reports GUI designer functionality.
	bool m_designMode;
};

}}}

#endif // QF_QMLWIDGETS_REPORTS_REPORTPROCESSOR_H

