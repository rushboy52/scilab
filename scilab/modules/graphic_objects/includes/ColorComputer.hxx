/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef COLORCOMPUTER_HXX
#define COLORCOMPUTER_HXX

/**
 * ColorComputer class
 * Various utility functions used to compute colors.
 * To be extended.
 */
class ColorComputer
{

public :

    /**
     * Ouputs an RGB color mapped to a scalar value s belonging to an [smin, smax] interval.
     * The output color is looked up in an RGB colormap, using a linear mapping between the latter and s.
     * It does not currently check whether srange is greater than 0.
     * If s is a Nan value, the black color is output.
     * @param[in] the scalar value.
     * @param[in] the interval's minimum value.
     * @param[in] the interval's range (smax-smin).
     * @param[in] an offset added to the index computed from s.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getColor(double s, double smin, double srange, double indexOffset, double* colormap, int colormapSize, float* returnedColor);

    /**
     * Outputs an RGB color mapped to a scalar value s belonging to an [smin, smax] interval.
     * The output color is looked up in an RGB colormap, using a linear mapping between a sub-interval of the latter and s.
     * It neither checks whether srange is greater than 0 nor verifies that the sub-interval is included within the colormap's
     * range (which must be ensured when specifying the sub-interval's bounds).
     * If s is a Nan value, the black color is output.
     * @param[in] the scalar value.
     * @param[in] the interval's minimum value.
     * @param[in] the interval's range (smax-smin).
     * @param[in] an offset added to the index computed from s.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[in] the index specifying the colormap sub-interval's lower bound.
     * @param[in] the index specifying the colormap sub-interval's upper bound.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getColor(double s, double smin, double srange, double indexOffset, double* colormap, int minIndex, int maxIndex,
        int colormapSize, float* returnedColor);

    /**
     * Outputs an RGB color directly mapped to a scalar value s.
     * The output color is looked up in an RGB colormap, using s as a direct index.
     * White and black are respectively output when s <= -3 and -3 < s < 0 ; s is also
     * clamped to the colormap's upper bound (colormapSize-1). If s is a Nan value, black is also
     * output.
     * @param[in] the scalar value used as an index.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getDirectColor(double s, double* colormap, int colormapSize, float* returnedColor);

    /**
     * Outputs an RGB color directly mapped to a scalar value s.
     * The output color is looked up in an RGB colormap, using s as a direct index,
     * which is clamped to the colormap bounds, that is [0, colormapSize-1] . If s is a Nan value,
     * the black color is output.
     * @param[in] the scalar value used as an index.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getClampedDirectColor(double s, double* colormap, int colormapSize, float* returnedColor);
};

/**
 * Offset passed to the getColor function for colors mapped to z values.
 */
#define Z_COLOR_OFFSET    0.5

/**
 * Offset passed to the getColor function for linearly mapped colors.
 */
#define COLOR_OFFSET      0.1

/**
 * Special color index values.
 */
enum SpecialColorIndexValues {
    WHITE_LOWER_INDEX = -4,
    BLACK_LOWER_INDEX = -3,
    BLACK_UPPER_INDEX = 0
};

/**
 * The minimum value of a single R, G or B component.
 */
#define MIN_COMPONENT_VALUE    0.0

/**
 * The maximum value of a single R, G or B component.
 */
#define MAX_COMPONENT_VALUE    1.0

#endif