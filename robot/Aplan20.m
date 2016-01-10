function [X_aix,Y_aix,Angle] = Aplan(x0,y0,x1,y1,radius,theta,model,mask_self_span,fax_self_angle)
%���벽����5mm
Aroute=fopen('D:\verson5.txt','a+');
dis_step = 5;
arc_step = 5;
if(model == 1)%ģʽΪֱ�߶�
    n = fix(sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)))/dis_step;
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
                if(mask_self_span == 1)
                    self_angle = fax_self_angle;
                else
                    self_angle = Angle(i+1)-pi/2;
                end
                fprintf(Aroute,'{ {%f,%f},%f,%f,%f},\n ',X_aix(i+1),Y_aix(i+1),Angle(i+1),self_angle,500.00);
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
            if(mask_self_span == 1)
                self_angle = fax_self_angle;
            else
                self_angle = Angle(i+1)-pi/2;
            end
            fprintf(Aroute,'{ {%f,%f},%f,%f,%f},\n ',X_aix(i+1),Y_aix(i+1),Angle(i+1),self_angle,500.00);
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
        
        display(['k=',num2str(k),'alpha = ', num2str(alpha)]);
        display(['a(1)=',num2str(a(1)),'a(2) = ', num2str(a(2))]);
    else
        if(y0 < y1)
            alpha = pi/2;
            a = [0 1];
        else
            alpha = -pi/2;
            a = [0 -1];
        end
    end
    
    for i = 1:1:n
        X_aix(i+1) = x0 + radius*cos(alpha+i*5*dir/radius);
        Y_aix(i+1) = y0 + radius*sin(alpha+i*5*dir/radius);
        Angle(i+1) = dir*pi/2 + (alpha+i*5*dir/radius);
        num = fix(Angle(i+1)/(pi));
        Angle(i+1) = Angle(i+1)- num*2*pi; 
        %display(['num=',num2str(num),'Angle = ', num2str(Angle(i+1))]);
        if(mask_self_span == 1)
            self_angle = fax_self_angle;
        else
            self_angle = Angle(i+1)-pi/2;
        end
        fprintf(Aroute,'{ {%f,%f},%f,%f,%f},\n ',X_aix(i+1),Y_aix(i+1),Angle(i+1),self_angle,500.00);
        plot(X_aix(i+1),Y_aix(i+1),'ro','EraseMode','background','MarkerSize',5);
        hold on
    end
end
fclose(Aroute);