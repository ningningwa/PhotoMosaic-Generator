/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"
#include "kdtree.h"
#include "point.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
     // if (theSource == NULL) {
     //   return NULL;
     // }
     map<Point<3>, TileImage> colorTile;
     vector<Point<3>> topush;
     unsigned i = 0;
     while (i < theTiles.size()) {
       colorTile[convertToXYZ(theTiles[i].getAverageColor())] = theTiles[i];
       topush.push_back(convertToXYZ(theTiles[i].getAverageColor()));
       i++;
     }
     KDTree<3> tree(topush);
     MosaicCanvas *go = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
     for (int i = 0; i < theSource.getRows(); i++) {
       for (int j = 0; j < theSource.getColumns(); j++) {
         Point<3> trytry = tree.findNearestNeighbor(Point<3>(theSource.getRegionColor(i, j).l, theSource.getRegionColor(i, j).u, theSource.getRegionColor(i, j).v));
         TileImage *gogo = new TileImage(colorTile[trytry]);
         go->setTile(i, j, gogo);
       }
     }
    return go;
}
