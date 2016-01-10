function [X_aix,Y_aix,Angle] = Aplan(x0,y0,x1,y1,radius,theta,model)
%���벽����5mm
Aroute=fopen('D:\first1.txt','a+');
dis_step = 5;
arc_step = 5;
if(model == 1)%ģʽΪֱ�߶�
    n = floor(sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)))/dis_step;
    X_aix(1) = x0;%�洢���λ��
    Y_aix(1) = y0;
    if(x1 - x0 ~= 0)%k����
        k = (y1-y0)/(x1-x0);%б�����
        if(y1-y0 ~= 0)
            a = [(x1-x0)/abs(x1-x0) (y1-y0)/abs(y1-y0)];%����
            if(a(1)==1 && a(2)==1)%��һ����
                alpha = atan(k);
            elseif(a(1)==-1 && a(2)==1)%�ڶ�����
                alpha = pi+atan(k);
            elseif(a(1)==1 && a(2)==-1)%��������
                alpha = atan(k);
            else%��������
                alpha = atan(k)-pi;
            end
        else
            a = [(x1-x0)/abs(x1-x0) 0];
            if(a(1)>0)
                alpha = 0;
            else
                alpha = pi;
            end
        end
        
        for i = 1:1:n
                X_aix(i+1) = x0+5*i*abs(cos(alpha))*a(1);
                Y_aix(i+1) = y0+5*i*abs(sin(alpha))*a(2);
                Angle(i+1) = alpha;
                fprintf(Aroute,'{ {%f,%f},%f,%f,%f},\n ',X_aix(i+1),Y_aix(i+1),Angle(i+1),0,500.00);
                plot(X_aix(i+1),Y_aix(i+1),'go','EraseMode','background','MarkerSize',5);
                hold on
        end
    else%k������
        for i = 1:1:n
            if(y0 < y1)%��������
                Y_aix(i+1) = y0 + 5*i;
                Angle(i+1) = pi/2;
            else%��������
                Y_aix(i+1) = y0 - 5*i;
                Angle(i+1) = -pi/2;
            end
            X_aix(i+1) = x0;
            fprintf(Aroute,'{ {%f,%f},%f,%f,%f},\n ',X_aix(i+1),Y_aix(i+1),Angle(i+1),0,500.00);
            plot(X_aix(i+1),Y_aix(i+1),'go','EraseMode','background','MarkerSize',5);
            hold on
        end
    end

else%ģʽΪԲ���߶�
    n = radius * abs(theta)/arc_step;%����㳤
    dir = theta/abs(theta);%˳ʱ�������ʱ��ת��
    X_aix(1) = x1;%�洢���λ��
    Y_aix(1) = y1;
    if(x1 - x0 ~= 0)
        k = (y1-y0)/(x1-x0);%��ʼ���Բ��б�����
        if(y1-y0 ~= 0)
            a = [(x1-x0)/abs(x1-x0) (y1-y0)/abs(y1-y0)];%����
        else
            a = [(x1-x0)/abs(x1-x0) 1];
        end
        alpha = atan(k);
        display(['k=',num2str(k),'alpha = ', num2str(alpha)]);
        display(['a(1)=',num2str(a(1)),'a(2) = ', num2str(a(2))]);
    else
        if(y0 < y1)
            alpha = pi/2;
            a(1) = 1;
            a(2) = 1;
        else
            alpha = -pi/2;
            a(1) = 1;
            a(2) = 1;
        end
    end
    for i = 1:1:n
        X_aix(i+1) = x0 + radius*cos(alpha+i*5*dir/radius)*a(1);
        Y_aix(i+1) = y0 + radius*sin(alpha+i*5*dir/radius)*a(2);
        Angle(i+1) = pi/2 + dir*(alpha+i*5/radius);
        fprintf(Aroute,'{ {%f,%f},%f,%f,%f},\n ',X_aix(i+1),Y_aix(i+1),Angle(i+1),0,500.00);
        plot(X_aix(i+1),Y_aix(i+1),'ro','EraseMode','background','MarkerSize',5);
        hold on
    end
end
fclose(Aroute);