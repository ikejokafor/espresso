import sys
sys.path.append('/home/ikenna/caffe-master/python/')
sys.path.append('/home/ikenna/detector_test_kitti/solo2/python/')
import numpy as np
import caffe
import cv2
import math
import argparse

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('path2deploy',
                        help="Path to deploy.prototxt")
    parser.add_argument('path2modelfile',
                        help="Path to .caffemodel")
    parser.add_argument('endlayer',
                    help="Layer to go up to")
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_args()
    path2deploy = args.path2deploy
    path2modelfile = args.path2modelfile
    endlayer = args.endlayer   
    
    net = caffe.Net(path2deploy, path2modelfile, caffe.TEST)
    frame = cv2.imread('/home/ikenna/detector_test_kitti/KITTI/000000.png')
    frame = cv2.resize(frame, (net.blobs['data'].shape[2], net.blobs['data'].shape[3]), cv2.INTER_LINEAR)
    cv2.imwrite('image.bmp', frame);
    net.blobs['data'].reshape(1,3,frame.shape[0],frame.shape[1])
    net.blobs['data'].data[...] = frame.transpose(2,0,1).astype(float)

    net_output = net.forward(start='conv1', end=endlayer)   
    print net_output[endlayer].shape    
    fh = open('output.txt', 'w')
    for i in range(0, net_output[endlayer].shape[1]):
          for j in range(0, net_output[endlayer].shape[2]):
              for k in range(0, net_output[endlayer].shape[3]):
                  fh.write(str(net_output[endlayer].item(0, i, j, k)) + ' ')
              fh.write('\n')
          fh.write('\n')
          fh.write('\n')
          fh.write('\n')
    fh.close()      
    quit()
