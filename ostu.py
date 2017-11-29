import SimpleITK as sitk
import numpy as np

imagename='s104510_enhanced_3.mha'
image = sitk.ReadImage(imagename)
print(image.GetSize())
image=sitk.Expand(image, [2,2,1], sitk.sitkLinear)
print(image.GetSize())
np=sitk.GetArrayFromImage(image)
del image
np[np>=0.01]=1
np[np<0.01]=0
outputname='s104510_enhuanced2_threshold.mha'
sitk.WriteImage(sitk.GetImageFromArray(np),outputname)
