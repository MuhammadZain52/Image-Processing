
BlackWhitePatch=zeros(4,4)
BlackWhitePatch(1,1)=255;
BlackWhitePatch(1,2)=255;
BlackWhitePatch(2,1)=255;
BlackWhitePatch(2,2)=255;
BlackWhitePatch(3,3)=255;
BlackWhitePatch(3,4)=255;
BlackWhitePatch(4,3)=255;
BlackWhitePatch(4,4)=255;
line=zeros(16,64);
x=BlackWhitePatch;
line = [x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x;x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x;x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x;x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x];
line2 = zeros(16,256);
line2 = [line,line,line,line];
MatchingGrayLevel= zeros(256,256);
gray_MatchingGrayLevel = zeros(16,256);
gray_MatchingGrayLevel(1:16,1:256) = 210;
MatchingGrayLevel=[line2;gray_MatchingGrayLevel;line2;gray_MatchingGrayLevel;line2;gray_MatchingGrayLevel;line2;gray_MatchingGrayLevel;line2;gray_MatchingGrayLevel;line2;gray_MatchingGrayLevel;line2;gray_MatchingGrayLevel;line2;gray_MatchingGrayLevel];
imshow(MatchingGrayLevel,[0,256]),title('MatchingGrayLevel'),colormap('gray');