int[] nodeCode;
Tree t;

void setup(){
	size(3000,800);
  String[] lines=loadStrings("treeSample.txt");
  
  for(int i=1;i<lines.length;i++){
    background(255);
    nodeCode=int(splitTokens(lines[i]," "));
    loadTree();
    t.drawTree();
    saveFrame("tree_"+i+".png");
  }
    
  
}

void draw(){
	exit();
}

void loadTree(){
	int size;
  size=nodeCode[0];
	t=new Tree(size);
	for(int i=1;i<=size;i++){
		t.readNode(nodeCode[i]);
	}
	t.complete();
}



class Tree{
	int size;
	int h;//1 for 1
	Node root;
	Tree(int i){
		size=i;
		h=0;
		root=null;
	}
	void readNode(int code){
		if(root==null){
			root=new Node(code,null);
			h=1;
		}
		else{
			Node cur;
			Node next=root;
			int curh=1;
			do{
				curh++;
				cur=next;
				if(abs(code)<abs(cur.value))next=cur.l;
				else if(abs(code)>abs(cur.value))next=cur.r;
				else break;
			}
			while(next!=null);
			
			if(code!=cur.value&&curh>h)h++;
			cur.append(code);
			println(cur.value,"->",code);
		}
		

	}
	void complete(){
		root.complete(h);
	}
	void printNode(){
		root.printSelf();
	}

	void drawTree(){
		float d0=width*0.75/(pow(2,h)-1);
		float h0=height*0.75/(h-1);
		//float h0=4*d0;
		float x0=width/8.0;
		float y0=h0*(h-1)/2.0+height/2.0;
		float r=width/500;
		Node cur=root;
		while(cur.l!=null)cur=cur.l;
		Node leftPivot=cur;



		for(int i=0;i<pow(2,h-2);i++){
			ellipseMode(CENTER);
			noStroke();
			if(cur.value==0)fill(255,0);
			else if(cur.value>0)fill(0);
			else fill(170,0,0);
			ellipse(x0+4*i*d0,y0,2*r,2*r);
			cur.x=x0+4*i*d0;
			if(cur.num>1){
				for(int j=0;j<cur.num-1;j++)ellipse(cur.x+0.7*d0+j*d0/2,y0,1.5*r,1.5*r);
			}

			cur=cur.next();
			if(cur.value==0)fill(255,0);
			else if(cur.value>0)fill(0);
			else fill(170,0,0);
			ellipse(x0+4*i*d0+3*d0, y0, 2*r, 2*r);
			cur.x=x0+4*i*d0+3*d0;
			if(cur.num>1){
				for(int j=0;j<cur.num-1;j++)ellipse(cur.x+0.7*d0+j*d0/2,y0,1.5*r,1.5*r);
			}
			cur=cur.next();
		}
		while(leftPivot!=null){
			//try{
   			//	Thread.sleep(1000);//1000ms = 1sec
			//}catch(InterruptedException e){
			//}
			leftPivot=leftPivot.father;
			cur=leftPivot;
			y0-=h0;
			while(cur!=null){
				
				cur.x=cur.xg();
				if(cur.value!=0){
					stroke(0);
					strokeWeight(3);
					if(cur.l!=null){
						if(cur.l.value<0){
							stroke(170,0,0);
							strokeWeight(4);
						}
						line(cur.x,y0,cur.l.x,y0+h0);
					}
					stroke(0);
					strokeWeight(3);
					if(cur.r!=null){
						if(cur.r.value<0){
							stroke(170,0,0);
							strokeWeight(4);
						}
						line(cur.x,y0,cur.r.x,y0+h0);
					}
				}

				ellipseMode(CENTER);
				noStroke();
				if(cur.value==0)fill(255,0);
				else if(cur.value>0)fill(0);
				else fill(170,0,0);
				ellipse(cur.x,y0,2*r,2*r);
				if(cur.num>1){
					for(int j=0;j<cur.num-1;j++)ellipse(cur.x+0.7*d0+j*d0/2,y0,1.5*r,1.5*r);
				}
	
				cur=cur.next();
			}

		}

	}
}

class Node{

	int value;
	Node l;
	Node r;
	Node father;
	int num;
	float x;
	Node(int code,Node up){
		value=code;
		l=null;
		r=null;
		num=1;
		father=up;
	}
	void append(int code){
		if(code==value)num++;
		else if(abs(code)>abs(value)){
			r=new Node(code,this);
		}
		else{
			l=new Node(code,this);
		}
	}
	int xsize(){
		int size=1;
		if(l!=null)size+=l.xsize();
		if(r!=null)size+=r.xsize();
		return size;
	}
	float xweight(){
		float sum=x;
		if(l!=null)sum+=l.xweight();
		if(r!=null)sum+=r.xweight();
		return sum;
	}
	float xg(){
		return 1.0*(l.xweight()+r.xweight())/(this.xsize()-1);
	}
	Node newCopy(){
		Node ret=new Node(value,null);
		ret.l=l;
		ret.r=r;
		ret.father=father;
		ret.num=num;
		ret.x=x;
		return ret;
	}
	void complete(int h){
		if(h>1){
			if(l==null)l=new Node(0,this);
			if(r==null)r=new Node(0,this);
			l.complete(h-1);
			r.complete(h-1);
		}
		
	}
	Node next(){
		if(father==null)return null;
		if(father.r==this){
			Node temp=father.next();
			if(temp==null)return null;
			else return temp.l;
		}
		else return father.r;
	}
	void printSelf(){
		println(value);
		if(this.l!=null)this.l.printSelf();
		if(this.r!=null)this.r.printSelf();
	}

}