x=imread('color.tif');
N=max(x(:));
image(x); 
colormap(rand(N,3));
axis('image');
