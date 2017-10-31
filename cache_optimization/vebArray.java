/**
 * Created by Ziaul on 3/7/2017.
 */
public class vebArray {

    protected TreeNode[] vEBNodes;
    int Size;
    int counter=0;
    public vebArray(int size)
    {
        Size = size;
        vEBNodes = new TreeNode[Size];

    }
    public void buildvEB(TreeNode[] nodes, int startIndex, int endIndex, int height)
    {
        if(height<=2)
        {
            if(height==2)
            {
                TreeNode root = nodes[startIndex+1];
                TreeNode left= nodes[startIndex];
                TreeNode right= nodes[endIndex];
                root.vEBIndex=counter;
                vEBNodes[counter++]=root;
                left.vEBIndex=counter;
                vEBNodes[counter++]=left;
                right.vEBIndex=counter;
                vEBNodes[counter++]=right;
                //System.out.print("[ " +root.pIndex + " " + left.pIndex + "  " + right.pIndex +"] ");
            }
            else
            {
                nodes[startIndex].vEBIndex=counter;
                vEBNodes[counter++]=nodes[startIndex];
                //System.out.print(" ["+nodes[startIndex].pIndex+"]" );
            }
            return;
        }
        TreeNode[] rootBlock = new TreeNode[Constants._MAX_ARRAY_SIZE];
        int count=0;
        int upperHeight = Math.floorDiv(height, 2);
        int lowerHeight = height - upperHeight;
        int stepSize = (int)Math.pow(2, lowerHeight)-1;
        for(int i=startIndex ; i<endIndex-stepSize; i+=stepSize)
        {
            int index = i+stepSize;
            rootBlock[count++] = nodes[index];
            i++;
        }

        buildvEB(rootBlock, 0, count-1, upperHeight);

        for(int i=startIndex;i<endIndex;)
        {
            buildvEB(nodes, i, i+stepSize-1, lowerHeight);
            i+=stepSize+1;
        }
    }

    public void printvEB()
    {
        for(int i=0;i<counter;i++)
        {
            System.out.print(vEBNodes[i].pIndex+ " ");
        }
    }

    public int getNodeIndex(TreeNode node)
    {
        for(int i=0;i<counter;i++)
        {
            if(vEBNodes[i]==node)
                return i;
        }
        return -1;
    }

    public TreeNode[] getvEBArray()
    {
        return vEBNodes;
    }

    public void makeTree(int[][] vEBTree)
    {

        int c=0;
        for (TreeNode node : vEBNodes)
        {
            if(node==null)
                continue;
            vEBTree[c][0]=node.m_key;
            vEBTree[c][1]=getNodeIndex(node.m_leftChild);
            vEBTree[c][2]=getNodeIndex(node.m_rightChild);
            c++;
        }
    }

    public int getPackedIndex()
    {
        return 0;
    }

    public TreeNode getTreeNode(int index)
    {
        return vEBNodes[index];
    }
  
    public boolean contains()
    {
        return false;
    }
}
