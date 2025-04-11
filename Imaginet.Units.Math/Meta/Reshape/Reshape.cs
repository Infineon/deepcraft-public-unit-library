/*
* Imagimob AB CONFIDENTIAL
* Unpublished Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* NOTICE: All information contained herein is, and remains the property of Imagimob AB.
*
* File created: 2020-09-03 Albert Seward (albert.seward@imagimob.com)
*/

using System;
using System.Collections.Immutable;
using System.Linq;
using Imagimob;
using Imaginet.Viper;
using Imaginet.Viper.Object.Expressions;
using Imaginet.Viper.Parser;

namespace Imaginet.Units.Math.Reshape;

public static class Reshape
{
    public static void Build(ITensor input, Shape shape, string axisNames, string axisLabels, out ITensor output)
    {
        shape ??= input.Shape;

        #region This is here only for backward compbalibity - After some time have passed, this can be removed.

        if (!string.IsNullOrWhiteSpace(axisLabels))
        {
            var allIndexNames = AxisLabelParser.ParseAxisLabels(axisLabels);

            if (allIndexNames.Length != shape.Rank)
                throw new ImaginetException($"The number of index groups don't match the tensor rank. Expected {shape.Rank}, got {allIndexNames.Length}");

            for (var axis = 0; axis < shape.Rank; axis++)
            {
                var indexNames = allIndexNames[axis];

                if (indexNames.Length != shape[axis].Size)
                    throw new ImaginetException($"The number of index names in axis {axis}. Expected {shape[axis].Size}, got {indexNames.Length}");

                shape[axis] = shape[axis] with { IndexNames = indexNames.ToImmutableList() };
            }
        }

        if (!string.IsNullOrWhiteSpace(axisNames))
        {
            var axisNamesList = Split(axisNames);

            if (axisNamesList.Length != shape.Rank)
                throw new ImaginetException($"The number of named dimensions don't match tensor rank. Expected {shape.Rank}, got {axisNamesList.Length}");

            for (int axis = 0; axis < shape.Rank; axis++)
            {
                shape[axis] = shape[axis] with { Name = axisNamesList[axis] };
            }
        }

        #endregion

        output = new TensorView(input, shape);
    }

    private static string[] Split(string str)
    {
        return str.Trim().Split(',', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries).Select(x => x switch
        {
            ['"', .. var literal, '"'] => literal,
            _ => x
        }).ToArray();
    }
}