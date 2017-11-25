import SimpleITK as sitk
import numpy as np
from frangi import frangi
import string

imagename='s104510.mha'
chars=string.split(imagename,'.')
name=chars[0]
suffix=chars[1]
image = sitk.ReadImage(imagename)
nda = sitk.GetArrayFromImage(image)

result=frangi(nda)
sitk.WriteImage(result, name+'_enhuanced.'+suffix)


