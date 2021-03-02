//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "../../3rd_party/polyline_simplification.h"
#include "../../engine/s3d/mesh.h"
#include "../compare.h"
#include "../imaging.h"
#include "color.h"
#include "image.h"
#include "point.h"
#include "pointf.h"
#include "polygonf.h"
#include "rect.h"
#include "rectf.h"

// Local Constants
const int c_neighbors =             5;                  // Number of neighbors to smooth points with


//####################################################################################
//##    Constructors
//####################################################################################
DrImage::DrImage(std::string image_name, DrBitmap &bitmap, float lod, bool outline) {
    this->m_simple_name = image_name;
    this->m_bitmap = bitmap;

    if (outline) {
        outlinePoints(lod);
    } else {
        m_poly_list.push_back(bitmap.polygon().points());
        m_hole_list.push_back({});
    }
}



//####################################################################################
//##    Sets Image Shape as simple box
//####################################################################################
void DrImage::setSimpleBox() {
    std::vector<DrPointF> one_poly = m_bitmap.polygon().points();
    std::vector<std::vector<DrPointF>> hole_list {{ }};
    DrPolygonF::ensureWindingOrientation(one_poly, Winding_Orientation::CounterClockwise);
    m_poly_list.clear();
    m_hole_list.clear();
    m_poly_list.push_back(one_poly);
    m_hole_list.push_back(hole_list);
    m_outline_canceled =  true;
    m_outline_processed = false;
}


//####################################################################################
//##    Loads list of points for Image and Image Holes
//##
//##    Level of Detail:
//##        0.075 = Detailed
//##        0.250 = Nice
//##        1.000 = Low poly
//##       10.000 = Really low poly
//##
//####################################################################################        
void DrImage::outlinePoints(float lod) {
    m_poly_list.clear();
    m_hole_list.clear();

    // ***** Break pixmap into seperate images for each object in image
    std::vector<DrBitmap>   bitmaps;
    std::vector<DrRect>     rects;
    bool    cancel = Dr::FindObjectsInBitmap(m_bitmap, bitmaps, rects, c_alpha_tolerance, true);
    int     number_of_objects = static_cast<int>(bitmaps.size());

    //std::cout << "Number of objects in image: " << number_of_objects << std::endl;

    // ***** If Find Objects In Bitmap never finished, just add simple box shape
    if (cancel) { setSimpleBox(); return; }

    // ******************** Go through each image (object) and Polygon for it
    for (int image_number = 0; image_number < number_of_objects; image_number++) {
        // Grab next image, check if its valid
        DrBitmap &image = bitmaps[image_number];
        if (image.width < 1 || image.height < 1) continue;

        // Trace edge of image
        std::vector<DrPointF> one_poly = Dr::TraceImageOutline(image);

        // Add rect offset, and add 1.00 pixels buffer around image
        double plus_one_pixel_percent_x = 1.0 + (1.00 / m_bitmap.width);
        double plus_one_pixel_percent_y = 1.0 + (1.00 / m_bitmap.height);
        for (auto &point : one_poly) {
            point.x += rects[image_number].left();
            point.y += rects[image_number].top();
            point.x = point.x * plus_one_pixel_percent_x;
            point.y = point.y * plus_one_pixel_percent_y;
        }

        // Remove duplicate first point
        if (one_poly.size() > 3) one_poly.pop_back();

        // Optimize point list
        if (one_poly.size() > (c_neighbors * 2)) {
            one_poly = DrMesh::smoothPoints(one_poly, c_neighbors, 20.0, 1.0);
            one_poly = PolylineSimplification::RamerDouglasPeucker(one_poly, lod);  
            //one_poly = DrMesh::insertPoints(one_poly);
        }

        // If we only have a couple points left, add shape as a box of the original image, otherwise use PolylineSimplification points
        if (one_poly.size() < 4) {
            ///points = HullFinder::FindConcaveHull(points, 5.0);
            one_poly.clear();
            one_poly.push_back( DrPointF(rects[image_number].topLeft().x,        rects[image_number].topLeft().y) );
            one_poly.push_back( DrPointF(rects[image_number].topRight().x,       rects[image_number].topRight().y) );
            one_poly.push_back( DrPointF(rects[image_number].bottomRight().x,    rects[image_number].bottomRight().y) );
            one_poly.push_back( DrPointF(rects[image_number].bottomLeft().x,     rects[image_number].bottomLeft().y) );
        }

        // Check winding
        DrPolygonF::ensureWindingOrientation(one_poly, Winding_Orientation::CounterClockwise);

        // Add polygon to list of polygons in shape
        m_poly_list.push_back(one_poly);


        // ******************** Copy image and finds holes as seperate outlines
        DrBitmap holes = image.copy();
        Dr::FillBorder(holes, Dr::white, holes.rect());                     // Ensures only holes are left as black spots

        // Breaks holes into seperate images for each Hole
        std::vector<DrBitmap> hole_images;
        std::vector<DrRect>   hole_rects;        
        Dr::FindObjectsInBitmap(holes, hole_images, hole_rects, c_alpha_tolerance, false);

        // Go through each image (Hole) create list for it
        std::vector<std::vector<DrPointF>> hole_list;
        for (int hole_number = 0; hole_number < static_cast<int>(hole_images.size()); hole_number++) {
            DrBitmap &hole = hole_images[hole_number];
            if (hole.width < 1 || hole.height < 1) continue;

            // Trace edge of hole
            std::vector<DrPointF> one_hole = Dr::TraceImageOutline(hole);

            // Add in sub image offset to points and hole rects
            for (auto &point : one_hole) {
                point.x += rects[image_number].left() + hole_rects[hole_number].left();
                point.y += rects[image_number].top()  + hole_rects[hole_number].top();
            }

            // Remove duplicate first point
            if (one_hole.size() > 3) one_hole.pop_back();

            // Optimize point list
            if (one_hole.size() > (c_neighbors * 2)) {
                one_hole = DrMesh::smoothPoints(one_hole, c_neighbors, 30.0, 1.0);
                one_hole = PolylineSimplification::RamerDouglasPeucker(one_hole, lod);
                //one_hole = DrMesh::insertPoints(one_hole);
            }

            if (one_hole.size() > 3) {
                DrPolygonF::ensureWindingOrientation(one_hole, Winding_Orientation::Clockwise);
                hole_list.push_back(one_hole);
            }
        }
        m_hole_list.push_back(hole_list);

    }   // End for each bitmap


    // ***** Mark this DrImage as having traced the image outline
    m_outline_canceled = false;
    m_outline_processed = true;


}   // End outlinePoints()









