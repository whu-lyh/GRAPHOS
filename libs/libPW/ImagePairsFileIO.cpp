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
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QObject>
#include <QStringList>
#include <QTextStream>
#include <QtXml>

#include "ImagePairsFileIO.h"
#include "OpenMVG/OpenMVGParametersDefinitions.h"

using namespace PW;

ImagePairsFileIO::ImagePairsFileIO()
{
}

ImagePairsFileIO::ImagePairsFileIO(QMap<QString, QVector<QString> > &imagePairs):
    mImagePairs(imagePairs)
{

}

int ImagePairsFileIO::getImagePairs(QMap<QString, QVector<QString> >& imagePairs)
{
    if(mImagePairs.isEmpty())
    {
        qCritical() << QObject::tr("Image pairs container is empty");
        return(IMAGEPAIRSFILEIO_ERROR);
    }
    imagePairs=mImagePairs;
    return(IMAGEPAIRSFILEIO_NO_ERROR);
}

int ImagePairsFileIO::readStandarAsciiFile(QString url)
{
    mImagePairs.clear();
    QFile file(url);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bool ok;
        int nLine=0;
        while (!file.atEnd())
        {
            nLine++;
            QString line = file.readLine();
            line=line.trimmed();
            if(line.isEmpty())
                continue;
            QStringList splitedLine = line.simplified().split(" ");
            if(splitedLine.count() == 2)
            {
                QString imageIdFirst=splitedLine.at(0).trimmed();
                QString imageIdSecond=splitedLine.at(1).trimmed();
                if(mImagePairs.contains(imageIdFirst))
                {
                    mImagePairs[imageIdFirst].push_back(imageIdSecond);
                }
                else
                {
                    QVector<QString> imageIds;
                    imageIds.push_back(imageIdSecond);
                    mImagePairs[imageIdFirst]=imageIds;
                }
                ok=true;
            }
            else ok = false;
            if(!ok)
            {
                qCritical() << QObject::tr("In line number: %1 in file:\n %2 \n there are not two strings")
                             .arg(nLine)
                             .arg(url);
                return(IMAGEPAIRSFILEIO_ERROR);
            }
        }
    }
    return(IMAGEPAIRSFILEIO_NO_ERROR);
}
/*
int ImagePairsFileIO::readXmlFile(QString imagePairsFileName)
{
    mImagePairs.clear();
    QFile file(imagePairsFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qCritical() << QObject::tr("ImagePairsFileIO::readXmlFile, error opening file:\n%1").arg(imagePairsFileName);
        return(IMAGEPAIRSFILEIO_ERROR);
    }
    QString errorStr;
    QString noStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file,true,&errorStr,&errorLine,&errorColumn))
    {
        QString strError=QObject::tr("ImagePairsFileIO::readXmlFile, error reading file:\n%1").arg(imagePairsFileName);
        strError+=QObject::tr("\nError: %1 in line %2 in column %3").arg(errorStr).arg(QString::number(errorLine)).arg(QString::number(errorColumn));
        qCritical() <<strError;
        return(IMAGEPAIRSFILEIO_ERROR);
    }

    QDomNodeList pairNodes=doc.elementsByTagName(OPENMVG_KPM_PROGRAM_XMLMATCHESFILE_TAG_PAIR);
    if(pairNodes.size()==0)
    {
        QString strError=QObject::tr("ImagePairsFileIO::readXmlFile, error in file:\n%1").arg(imagePairsFileName);
        strError+=QObject::tr("\nNo tag pairs in the file");
        qCritical() <<strError;
        return(IMAGEPAIRSFILEIO_ERROR);
    }
    for(int nPairNode=0;nPairNode<pairNodes.size();nPairNode++)
    {
        QDomNode pairNode=pairNodes.at(nPairNode);
        if(!pairNode.isElement())
        {
            QString strError=QObject::tr("ImagePairsFileIO::readXmlFile, error in file:\n%1").arg(imagePairsFileName);
            strError+=QObject::tr("\nPair node is not an element");
            qCritical() <<strError;
            return(IMAGEPAIRSFILEIO_ERROR);
        }
        QDomElement pairElement=pairNode.toElement();
        QString imageIdFirst=pairElement.attribute(OPENMVG_KPM_PROGRAM_XMLMATCHESFILE_ATTRIBUTE_FIRST_IMAGE);
        if(imageIdFirst.isEmpty())
        {
            QString strError=QObject::tr("ImagePairsFileIO::readXmlFile, error in file:\n%1").arg(imagePairsFileName);
            strError+=QObject::tr("\nThere is a pair with a first image attibute empty");
            qCritical() <<strError;
            return(IMAGEPAIRSFILEIO_ERROR);
        }
        QString imageIdSecond=pairElement.attribute(OPENMVG_KPM_PROGRAM_XMLMATCHESFILE_ATTRIBUTE_SECOND_IMAGE);
        if(imageIdSecond.isEmpty())
        {
            QString strError=QObject::tr("ImagePairsFileIO::readXmlFile, error in file:\n%1").arg(imagePairsFileName);
            strError+=QObject::tr("\nThere is a pair with a second image attibute empty");
            qCritical() <<strError;
            return(IMAGEPAIRSFILEIO_ERROR);
        }
        if(mImagePairs.contains(imageIdFirst))
        {
            mImagePairs[imageIdFirst].push_back(imageIdSecond);
        }
        else
        {
            QVector<QString> imageIds;
            imageIds.push_back(imageIdSecond);
            mImagePairs[imageIdFirst]=imageIds;
        }
    }
    return(IMAGEPAIRSFILEIO_NO_ERROR);
}
*/
int ImagePairsFileIO::writeTapiocaPairsFile(QString url, int duplicateReverse)
{
    if(mImagePairs.size()==0)
    {
        qCritical() << QObject::tr("Image pairs container is empty");
        return(IMAGEPAIRSFILEIO_ERROR);
    }
    QFile file(url);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << "<?xml version=\"1.0\" ?> \n";
    out << "<SauvegardeNamedRel>\n";
    QMap<QString, QVector<QString> >::const_iterator iter = mImagePairs.constBegin();
    while (iter != mImagePairs.constEnd())
    {
        QVector<QString> imageIds=iter.value();
        for(int nImageId=0;nImageId<imageIds.size();nImageId++)
        {
            out<<"<Cple>"<<iter.key()<<" "<<imageIds[nImageId]<<"</Cple>"<<endl;
            if(duplicateReverse==1)
                out<<"<Cple>"<<imageIds[nImageId]<<" "<<iter.key()<<"</Cple>"<<endl;
        }
        ++iter;
    }
    out << "</SauvegardeNamedRel>\n";
    return(IMAGEPAIRSFILEIO_NO_ERROR);
}

int ImagePairsFileIO::writeStandarAsciiPairsFile(QString url, int duplicateReverse)
{
    if(mImagePairs.size()==0)
    {
        qCritical() << QObject::tr("Image pairs container is empty");
        return(IMAGEPAIRSFILEIO_ERROR);
    }
    QFile file(url);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    QMap<QString, QVector<QString> >::const_iterator iter = mImagePairs.constBegin();
    while (iter != mImagePairs.constEnd())
    {
        QVector<QString> imageIds=iter.value();
        for(int nImageId=0;nImageId<imageIds.size();nImageId++)
        {
            out<<iter.key()<<" "<<imageIds[nImageId]<<"\n";
            if(duplicateReverse==1)
                out<<imageIds[nImageId]<<" "<<iter.key()<<"\n";
        }
        ++iter;
    }
    return(IMAGEPAIRSFILEIO_NO_ERROR);
}
