/**
******************************************************************************
*
* @file       core.h
* @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2012.
* @author     Tau Labs, http://taulabs.org, Copyright (C) 2013
* @brief      
* @see        The GNU Public License (GPL) Version 3
* @defgroup   TLMapWidget
* @{
* 
*****************************************************************************/
/* 
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
* for more details.
* 
* You should have received a copy of the GNU General Public License along 
* with this program; if not, write to the Free Software Foundation, Inc., 
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef CORE_H
#define CORE_H

#include "debugheader.h"

#include "pointlatlng.h"
#include "mousewheelzoomtype.h"
#include "core/size.h"
#include "core/point.h"

#include "core/maptype.h"
#include "rectangle.h"
#include "QThreadPool"
#include "tilematrix.h"
#include <QQueue>
#include "loadtask.h"
#include "copyrightstrings.h"
#include "rectlatlng.h"
#include "projections/lks94projection.h"
#include "projections/mercatorprojection.h"
#include "projections/mercatorprojectionyandex.h"
#include "projections/platecarreeprojection.h"
#include "projections/platecarreeprojectionpergo.h"
#include "core/geodecoderstatus.h"
#include "core/tlmaps.h"
#include "core/diagnostics.h"

#include <QSemaphore>
#include <QThread>
#include <QDateTime>

#include <QObject>
#include "core/corecommon.h"

namespace mapcontrol
{
    class TLMapControl;
    class MapGraphicItem;
}

namespace internals {

    class TLMAPWIDGET_EXPORT Core:public QObject,public QRunnable
    {
        Q_OBJECT

        friend class mapcontrol::TLMapControl;
        friend class mapcontrol::MapGraphicItem;
    public:
        Core();
        ~Core();
        void run();
        PointLatLng CurrentPosition()const{return currentPosition;}

        void SetCurrentPosition(const PointLatLng &value);

        core::Point GetcurrentPositionGPixel(){return currentPositionPixel;}
        void SetcurrentPositionGPixel(const core::Point &value){currentPositionPixel=value;}

        core::Point GetrenderOffset(){return renderOffset;}
        void SetrenderOffset(const core::Point &value){renderOffset=value;}

        core::Point GetcenterTileXYLocation(){return centerTileXYLocation;}
        void SetcenterTileXYLocation(const core::Point &value){centerTileXYLocation=value;}

        core::Point GetcenterTileXYLocationLast(){return centerTileXYLocationLast;}
        void SetcenterTileXYLocationLast(const core::Point &value){centerTileXYLocationLast=value;}

        core::Point GetdragPoint(){return dragPoint;}
        void SetdragPoint(const core::Point &value){dragPoint=value;}

        core::Point GetmouseDown(){return mouseDown;}
        void SetmouseDown(const core::Point &value){mouseDown=value;}

        core::Point GetmouseCurrent(){return mouseCurrent;}
        void SetmouseCurrent(const core::Point &value){mouseCurrent=value;}

        core::Point GetmouseLastZoom(){return mouseLastZoom;}
        void SetmouseLastZoom(const core::Point &value){mouseLastZoom=value;}

        MouseWheelZoomType::Types GetMouseWheelZoomType(){return mousewheelzoomtype;}
        void SetMouseWheelZoomType(const MouseWheelZoomType::Types &value){mousewheelzoomtype=value;}

        PointLatLng GetLastLocationInBounds(){return LastLocationInBounds;}
        void SetLastLocationInBounds(const PointLatLng &value){LastLocationInBounds=value;}

        Size GetsizeOfMapArea(){return sizeOfMapArea;}
        void SetsizeOfMapArea(const Size &value){sizeOfMapArea=value;}

        Size GetminOfTiles(){return minOfTiles;}
        void SetminOfTiles(const Size &value){minOfTiles=value;}

        Size GetmaxOfTiles(){return maxOfTiles;}
        void SetmaxOfTiles(const Size &value){maxOfTiles=value;}

        Rectangle GetTileRect(){return tileRect;}
        void SettileRect(const Rectangle &value){tileRect=value;}

        core::Point GettilePoint(){return tilePoint;}
        void SettilePoint(const core::Point &value){tilePoint=value;}

        Rectangle GetCurrentRegion(){return CurrentRegion;}
        void SetCurrentRegion(const Rectangle &value){CurrentRegion=value;}

        QList<core::Point> tileDrawingList;

        PureProjection* Projection()
        {
            return projection;
        }
        void SetProjection(PureProjection* value)
        {
            projection=value;
            tileRect=Rectangle(core::Point(0,0),value->TileSize());
        }
        bool IsDragging()const{return isDragging;}

        int Zoom()const{return zoom;}
        void SetZoom(int const& value);

        int MaxZoom()const{return maxzoom;}

        void UpdateBounds();

        MapType::Types GetMapType(){return mapType;}
        void SetMapType(MapType::Types const& value);

        void SetUserImageHorizontalScale(double hScale);
        void SetUserImageVerticalScale(double vScale);
        void SetUserImageLocation(QString mapLocation);

        void StartSystem();

        void UpdateCenterTileXYLocation();

        void OnMapSizeChanged(int const& width, int const& height);//TODO had as slot

        void OnMapClose();//TODO had as slot

        GeoCoderStatusCode::Types SetCurrentPositionByKeywords(QString const& keys);

        RectLatLng CurrentViewArea();

        PointLatLng FromLocalToLatLng(qint64 const& x, qint64 const& y);

        Point FromLatLngToLocal(PointLatLng const& latlng);

        int GetMaxZoomToFitRect(RectLatLng const& rect);

        void BeginDrag(core::Point const& pt);

        void EndDrag();

        void ReloadMap();

        void GoToCurrentPosition();

        void DragOffset(core::Point const& offset);

        void Drag(core::Point const& pt);

        void CancelAsyncTasks();

        void FindTilesAround(QList<core::Point> &list);

        void UpdateGroundResolution();

        TileMatrix Matrix;

        bool isStarted(){return started;}

        diagnostics GetDiagnostics();

        QList<UrlFactory::geoCodingStruct> GetAddressesFromCoordinates(PointLatLng coord, GeoCoderStatusCode::Types &status);
        QList<UrlFactory::geoCodingStruct> GetCoordinatesFromAddress(const QString &address, GeoCoderStatusCode::Types &status);
        double GetElevationFromCoordinates(PointLatLng coord, GeoCoderStatusCode::Types &status);
    signals:
        void OnCurrentPositionChanged(internals::PointLatLng point);
        void OnTileLoadComplete();
        void OnTilesStillToLoad(int number);
        void OnTileLoadStart();
        void OnMapDrag();
        void OnMapZoomChanged();
        void OnMapTypeChanged(MapType::Types type);
        void OnEmptyTileError(int zoom, core::Point pos);
        void OnNeedInvalidation();

    private:
        bool started;
        bool MouseWheelZooming;
        void keepInBounds();
        PointLatLng currentPosition;
        core::Point currentPositionPixel;
        core::Point renderOffset;
        core::Point centerTileXYLocation;
        core::Point centerTileXYLocationLast;
        core::Point dragPoint;
        Rectangle tileRect;
        core::Point mouseDown;
        bool CanDragMap;
        core::Point mouseCurrent;
        PointLatLng LastLocationInBounds;
        core::Point mouseLastZoom;

        MouseWheelZoomType::Types mousewheelzoomtype;

        QString userImageLocation;
        float userImageHorizontalScale;
        float userImageVerticalScale;

        Size sizeOfMapArea;
        Size minOfTiles;
        Size maxOfTiles;

        core::Point tilePoint;

        Rectangle CurrentRegion;

        QQueue<LoadTask> tileLoadQueue;

        int zoom;

        PureProjection* projection;

        bool isDragging;

        QMutex MtileLoadQueue;

        QMutex Moverlays;

        QMutex MtileDrawingList;
#ifdef DEBUG_CORE
        QMutex Mdebug;
        static qlonglong debugcounter;
#endif
        Size TooltipTextPadding;

        MapType::Types mapType;

        QSemaphore loaderLimit;

        QThreadPool ProcessLoadTaskCallback;
        QMutex MtileToload;
        int tilesToload;

        int maxzoom; //Max zoom level in  quadtile format
        QMutex MrunningThreads;
        int runningThreads;
        diagnostics diag;

    protected:
        qint64 Width;
        qint64 Height;
        int pxRes100m;  // 100 meters
        int pxRes1000m;  // 1km
        int pxRes10km; // 10km
        int pxRes100km; // 100km
        int pxRes1000km; // 1000km
        int pxRes5000km; // 5000km
        void SetCurrentPositionGPixel(core::Point const& value){currentPositionPixel = value;}
        void GoToCurrentPositionOnZoom();

    };

}
#endif // CORE_H
