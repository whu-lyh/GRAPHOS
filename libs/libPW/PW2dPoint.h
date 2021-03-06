/**
*-------------------------------------------------
*  Copyright 2016 by Tidop Research Group <daguilera@usal.se>
*
* This file is part of GRAPHOS - inteGRAted PHOtogrammetric Suite.
*
* GRAPHOS - inteGRAted PHOtogrammetric Suite is free software: you can redistribute
* it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either
* version 3 of the License, or (at your option) any later version.
*
* GRAPHOS - inteGRAted PHOtogrammetric Suite is distributed in the hope that it will
* be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*
* @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
*-------------------------------------------------
*/
#ifndef PW2DPOINT_H
#define PW2DPOINT_H

#include <QPointF>

#include "PWPoint.h"

namespace PW{


class LIBPWSHARED_EXPORT PW2dPoint : public PWPoint, public QPointF
{
public:
    PW2dPoint(PWPoint::PointType type = PWPoint::Control);
    PW2dPoint(double x, double y, QString name = "", PWPoint::PointType type = PWPoint::Control);

    /*!
     \brief Gets de number of components or dimensions of the point

     \return int
    */
    virtual int getCoordsCount();
    /*!
     \brief Gets the value of a component or coordinate.

     \param index
     \return double
    */
    virtual double getCoord(int index);

    /*!
     \brief Gets the 2d Point.

     \return QPointF
    */
    virtual QPointF getPoint();

    /*!
     \brief Sets the value of a component or coordinate.

     \param index
    */
    virtual void setCoord(int index, double value);


};

}
#endif // PW2DPOINT_H
