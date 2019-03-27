/*****************************************************************************/
/*  list.cpp - List selected entities                                        */
/*                                                                           */
/*  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/


#include <QTextEdit>
//#include <QColor>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <cmath>
#include "liststrip.h"

// yangbin
#ifndef M_PI
#define M_PI       3.141592653589793238462643
#endif
#ifndef M_PI_2
#define M_PI_2       1.57079632679489661923
#endif
#define M_PIx2      6.283185307179586 // 2*PI

QString LC_List::name() const
 {
     return (tr("List strip entities"));
 }

PluginCapabilities LC_List::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("plugins_menu", tr("List strip entities"));
    return pluginCapabilities;
}

void LC_List::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(parent);
    Q_UNUSED(cmd);
    d = doc;
    QList<Plug_Entity *> obj;
    bool yes  = doc->getSelect(&obj);
    if (!yes || obj.isEmpty()) return;

	//	柱下板带
	std::vector<StripData>  strips;
	for (int i = 0; i < obj.size(); ++i) {
		filterData1(obj.at(i), strips);
	}
	for (int i = 0; i < obj.size(); ++i) {
		filterData2(obj.at(i), strips);
	}
	for (int i = 0; i < obj.size(); ++i) {
		filterData3(obj.at(i), strips);
	}

    QString text;
    for (int i = 0; i < strips.size(); ++i) {
        text.append( QString("%1 %2: ").arg(tr("n")).arg(i+1));
        text.append( getStrData(strips[i]));
        text.append( "\n");
    }
    lc_Listdlg dlg(parent);
    dlg.setText(text);
    dlg.exec();

    while (!obj.isEmpty())
        delete obj.takeFirst();
}

bool LC_List::sign(const QPointF& v1, const QPointF& v2, const QPointF& v3) {
	double res = (v1.x() - v3.x())*(v2.y() - v3.y()) - (v2.x() - v3.x())*(v1.y() - v3.y());
	return (res >= 0.0);
}

/**
* 计算向量的夹角 (in rad).
*/
double angle(double x, double y) {
	return fmod(M_PI + remainder(atan2(y, x) - M_PI, m_piX2), m_piX2);
}

void LC_List::filterData1(Plug_Entity *ent, std::vector<StripData>& strips) {
	if (NULL == ent)
		return ;

	QHash<int, QVariant> data;
	//common entity data
	ent->getData(&data);

	//specific entity data
	int et = data.value(DPI::ETYPE).toInt();
	switch (et) {
	case DPI::POLYLINE: {
		QString strLayer = data.value(DPI::LAYER).toString();
		if (strLayer.indexOf("z柱下板带") >= 0) {
			StripData strip;
			strip.strLayer = data.value(DPI::LAYER).toString();
			strip.strColor = ent->intColor2str(data.value(DPI::COLOR).toInt());

			QList<Plug_VertexData> vl;
			ent->getPolylineData(&vl);
			int iVertices = vl.size();
			for (int i = 0; i < iVertices; ++i) {
				strip.vertexs.push_back(vl.at(i).point);
			}

			if (iVertices == 4) {
				double sizex = hypot(vl.at(0).point.x() - vl.at(1).point.x(), vl.at(0).point.y() - vl.at(1).point.y());
				double sizey = hypot(vl.at(1).point.x() - vl.at(2).point.x(), vl.at(1).point.y() - vl.at(2).point.y());
				if (sizex > sizey) {
					strip.sizeBig = sizex;
					strip.sizeSmall = sizey;

					strip.vertexsBig.push_back(vl.at(0).point);
					strip.vertexsBig.push_back(vl.at(1).point);

					strip.vertexsSmall.push_back(vl.at(1).point);
					strip.vertexsSmall.push_back(vl.at(2).point);
				}
				else {
					strip.sizeBig = sizey;
					strip.sizeSmall = sizex;

					strip.vertexsSmall.push_back(vl.at(0).point);
					strip.vertexsSmall.push_back(vl.at(1).point);

					strip.vertexsBig.push_back(vl.at(1).point);
					strip.vertexsBig.push_back(vl.at(2).point);
				}

				strips.push_back(strip);
			}
		}
		break; }
	default:
		break;
	}

}



void LC_List::filterData2(Plug_Entity *ent, std::vector<StripData>& strips) {
	if (NULL == ent)
		return;

	QPointF ptA;
	QHash<int, QVariant> data;
	//common entity data
	ent->getData(&data);

	//specific entity data
	int et = data.value(DPI::ETYPE).toInt();
	switch (et) {
	
	case DPI::POLYLINE: {
		QString strLayer = data.value(DPI::LAYER).toString();
		if (strLayer.indexOf("板底钢筋") >= 0) {
			
			QList<Plug_VertexData> vl;
			ent->getPolylineData(&vl);
			int iVertices = vl.size();
			
			if (iVertices == 4) {
				double sizex = hypot(vl.at(0).point.x() - vl.at(1).point.x(), vl.at(0).point.y() - vl.at(1).point.y());
				double sizey = hypot(vl.at(1).point.x() - vl.at(2).point.x(), vl.at(1).point.y() - vl.at(2).point.y());
				
				// 计算中间点, 并确定钢筋线方向
				QPointF middle, pa, pb;
				if (sizex > sizey) {
					middle.setX((vl.at(0).point.x() + vl.at(1).point.x()) / 2);
					middle.setY((vl.at(0).point.y() + vl.at(1).point.y()) / 2);
					pa = vl.at(0).point;
					pb = vl.at(1).point;
				}
				else {
					middle.setX((vl.at(1).point.x() + vl.at(2).point.x()) / 2);
					middle.setY((vl.at(1).point.y() + vl.at(2).point.y()) / 2);
					pa = vl.at(1).point;
					pb = vl.at(2).point;
				}

				// 匹配 柱中板带
				int iStrips = strips.size();
				for (int i = 0; i < iStrips; ++i) {
					QPointF p1 = strips[i].vertexs[0];
					QPointF p2 = strips[i].vertexs[1];
					QPointF p3 = strips[i].vertexs[2];
					QPointF p4 = strips[i].vertexs[3];

					bool bInside = false;

					bool s1 = sign(p1, p2, middle);
					bool s2 = sign(p2, p3, middle);
					bool s3 = sign(p3, p1, middle);
					if ((s1 == s2) && (s2 == s3))  bInside = true;

					if (!bInside) {
						s1 = sign(p2, p3, middle);
						s2 = sign(p3, p4, middle);
						s3 = sign(p4, p2, middle);
						if ((s1 == s2) && (s2 == s3))  bInside = true;
					}

					if (bInside) {
						QPointF dirBig = strips[i].vertexsBig[0] - strips[i].vertexsBig[1];
						QPointF dirSmall = strips[i].vertexsSmall[0] - strips[i].vertexsSmall[1];
						QPointF dirSteel = pa - pb;
						QPointF dirAngel = dirBig - dirSteel;
						double an = abs(angle(dirAngel.x(), dirAngel.y()));

						if (an < (M_PI / 6) || an >(M_PI * 5 / 6)) {
							// 钢筋线与板带的大边平行
							strips[i].vertexsSteelBig.push_back(pa);
							strips[i].vertexsSteelBig.push_back(pb);
						} 
						else {
							strips[i].vertexsSteelSmall.push_back(pa);
							strips[i].vertexsSteelSmall.push_back(pb);
						}
						break;
					}
				}
			}
		}
	}
		break; 
	default:
		break;
	}

}


void LC_List::filterData3(Plug_Entity *ent, std::vector<StripData>& strips) {
	if (NULL == ent)
		return;

	QPointF ptA;
	QHash<int, QVariant> data;
	//common entity data
	ent->getData(&data);

	//specific entity data
	int et = data.value(DPI::ETYPE).toInt();
	switch (et) {
	
	case DPI::INSERT: {
		ptA.setX(data.value(DPI::STARTX).toDouble());
		ptA.setY(data.value(DPI::STARTY).toDouble());

		QString strLayer = data.value(DPI::LAYER).toString();
		if ( true /*strLayer.indexOf("板底钢筋") >= 0*/) {

			QList<Plug_AttribData> vl;
			ent->getAttribData(&vl);
			int iVertices = vl.size();

			// 匹配 柱中板带
			int iStrips = strips.size();
			for (int i = 0; i < iStrips; ++i) {
				QPointF p1 = strips[i].vertexs[0];
				QPointF p2 = strips[i].vertexs[1];
				QPointF p3 = strips[i].vertexs[2];
				QPointF p4 = strips[i].vertexs[3];

				bool bInside = false;

				bool s1 = sign(p1, p2, ptA);
				bool s2 = sign(p2, p3, ptA);
				bool s3 = sign(p3, p1, ptA);
				if ((s1 == s2) && (s2 == s3))  bInside = true;

				if (!bInside) {
					s1 = sign(p2, p3, ptA);
					s2 = sign(p3, p4, ptA);
					s3 = sign(p4, p2, ptA);
					if ((s1 == s2) && (s2 == s3))  bInside = true;
				}

				if (bInside) {
					// 查找最近的钢筋线，以确定 钢筋标注

					if (strips[i].vertexsSteelBig.size() > 0 && strips[i].vertexsSteelSmall.size() > 0) {
						QPointF middleSteelBig, middleSteelSmall;
						middleSteelBig = (strips[i].vertexsSteelBig[0] + strips[i].vertexsSteelBig[1]) / 2;
						middleSteelSmall = (strips[i].vertexsSteelSmall[0] + strips[i].vertexsSteelSmall[1]) / 2;

						double distanceToBig = hypot(middleSteelBig.x() - ptA.x(), middleSteelBig.y() - ptA.y());
						double distanceToSmall = hypot(middleSteelSmall.x() - ptA.x(), middleSteelSmall.y() - ptA.y());
						if (distanceToBig < distanceToSmall) {
							strips[i].steelBig = vl.at(0).text;
						}
						else {
							strips[i].steelSmall = vl.at(0).text;
						}
					}
					else {

					}
					
					break;
				}
			}
		}
	}
		break;
	default:
		break;
	}

}


QString LC_List::getStrData(StripData strip) {
    

	QString strData(""),
		strEntity("%1\n"),
		strCommon("  %1: %2\n");
	
    
    strData  = strCommon.arg(tr("sizeBig")).arg(strip.sizeBig);
	strData.append(strCommon.arg(tr("sizeSmall")).arg(strip.sizeSmall));
	strData.append(strCommon.arg(tr("steelBig")).arg(strip.steelBig));

    return strData;
}

double LC_List::polylineRadius( const Plug_VertexData& ptA, const Plug_VertexData& ptB)
{
    double dChord = sqrt( pow(ptA.point.x() - ptB.point.x(), 2) + pow(ptA.point.y() - ptB.point.y(), 2));

    return fabs( 0.5 * dChord / sin( 2.0 * atan(ptA.bulge)));
}

/*****************************/
lc_Listdlg::lc_Listdlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("List strip entities"));
//    QTextEdit *edit= new QTextEdit(this);
    edit.setReadOnly (true);
    edit.setAcceptRichText ( false );
    QDialogButtonBox* bb = new QDialogButtonBox( QDialogButtonBox::Close, Qt::Horizontal, this );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(&edit);
    mainLayout->addWidget(bb);
    this->setLayout(mainLayout);
    this->resize ( 450, 350 );

    connect(bb, SIGNAL(rejected()), this, SLOT(accept()));
}

void lc_Listdlg::setText(QString text)
{
    edit.setText(text);
}
lc_Listdlg::~lc_Listdlg()
{
}
