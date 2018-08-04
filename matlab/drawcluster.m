X=real(particles);
Y=imag(particles);
L=max(max(abs(X)),max(abs(Y)));
X=[X,L,L,-L,-L];
Y=[Y,L,-L,L,-L];
C=linspace(1,10,length(X));
h=scatter(X,Y,[],C);
%currentunits = get(gca,'Units');
%set(gca, 'Units', 'Points');
%axpos = get(gca,'Position');
%set(gca, 'Units', currentunits);
%markerWidth = 1/diff(xlim)*axpos(3); % Calculate Marker width in points
%set(h, 'SizeData', markerWidth^2)
a=3