#include <bits/stdc++.h>

#include "ModelHelper.h"
#include "ShapeFeatureHelper.h"
#include "ParticleDensityHelper.h"
#include "VolumeHelper.h"
#include "SolidTextureHelper.h"

int main() {
    calculateShapeFeature();
    calculateParticleDensity();
    constructVolume();
    sampleSolidTexture();
    drawModel("model/cube.obj");
    return 0;
}
