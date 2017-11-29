import SimpleITK as sitk
import numpy as np
from frangi import frangi
import string

imagename='s104510.mha'
chars=string.split(imagename,'.')
name=chars[0]
suffix=chars[1]
image = sitk.ReadImage(imagename)
height=image.GetHeight()
width=image.GetWidth()

scale=[2,4,8]
enhanced_bank=[]
for i in scale:
	subsample=image[:width/(i),:height/(i),:]
	img_frangi=sitk.GetImageFromArray(frangi(sitk.GetArrayFromImage(subsample),(1,2)))
	result=sitk.Expand(img_frangi, [i,i,1], sitk.sitkLinear)
	outputname=name+'_enhanced_'+str(i)+'.'+suffix
	sitk.WriteImage(result,outputname)		
