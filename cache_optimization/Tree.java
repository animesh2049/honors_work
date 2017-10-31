import java.util.ArrayList;

/**
 * Created by Ziaul on 3/7/2017.
 */

public class Tree {
    PackedArray m_packed;
    public vebArray    m_veb;
    public TreeNode Root=null;
    private int[][] vEBTree;
    ArrayList<Integer> debug;
    int Size;

    public Tree(int size)
    {
        vEBTree = new int[size][3];
        Size = size;
        debug = new ArrayList<>();
        m_packed = new PackedArray(size);
        m_veb = new vebArray(size);
        linkNodes(0, size-1);
        m_veb.buildvEB(m_packed.getNodes(), 0, size-1, Constants._TreeHeight);
        m_veb.makeTree(vEBTree);
    }

    public void linkNodes(int startIndex, int endIndex)
    {
        if(startIndex>=endIndex || startIndex +1 ==endIndex )
            return;

        int pickIndex = startIndex + Math.floorDiv((endIndex-startIndex),2);
        TreeNode root = m_packed.getNode(pickIndex);
        if(Root==null)
            Root=root;

        int leftChild = startIndex + Math.floorDiv(pickIndex-1-startIndex,2);
        int rightChild = pickIndex + Math.floorDiv(endIndex - pickIndex+1,2) ;

        TreeNode leftNode, rightNode;
        leftNode = m_packed.getNode(leftChild);
        rightNode= m_packed.getNode(rightChild);
        leftNode.m_parent=root;
        rightNode.m_parent=root;
        root.m_leftChild=leftNode;
        root.m_rightChild=rightNode;

        linkNodes(startIndex, pickIndex-1);
        linkNodes(pickIndex+1, endIndex);
    }

    public boolean searchvEB(int key, int root)
    {
        if(root==-1)
        {
            return false;
        }
        if(vEBTree[root][0] == key)
        {
            System.out.println(" " + vEBTree[m_veb.getTreeNode(root).vEBIndex][0] + " " +vEBTree[root][0]+ "  " + m_veb.getTreeNode(root).m_key + " " + m_packed.getNode(m_veb.getTreeNode(root).pIndex).m_key);
            return true;
        }
        if(vEBTree[root][0] == 0)
        {
            if(key < m_veb.getTreeNode(vEBTree[root][1]).heapValue)
                searchvEB(key, vEBTree[root][1]);
            else
                searchvEB(key, vEBTree[root][2]);

        }
        else
        {
            if(key > vEBTree[root][0])
                searchvEB(key, vEBTree[root][2]);
            else
                searchvEB(key, vEBTree[root][1]);
        }
        return false;

    }

    public TreeNode getPred(int key, int root)
    {
        TreeNode pred = null;
        if(vEBTree[root][1]==-1 && vEBTree[root][2]==-1)
        {
            return m_veb.getTreeNode(root);
        }
        if(vEBTree[root][0] == key)
        {
            System.out.println(" " + vEBTree[m_veb.getTreeNode(root).vEBIndex][0] + " " +vEBTree[root][0]+ "  " + m_veb.getTreeNode(root).m_key + " " + m_packed.getNode(m_veb.getTreeNode(root).pIndex).m_key);
            System.out.println(" key already present ");
            return null;
        }
        if(vEBTree[root][0] == 0)
        {
            if(key < m_veb.getTreeNode(vEBTree[root][1]).heapValue)
                pred=getPred(key, vEBTree[root][1]);
            else
                pred=getPred(key, vEBTree[root][2]);

        }
        else
        {
            if(key > vEBTree[root][0])
                pred =getPred(key, vEBTree[root][2]);
            else
                pred = getPred(key, vEBTree[root][1]);
        }
        return pred;
    }
    public void heapifyAll(TreeNode node)
    {
        if(node==null)
            return;
        heapifyAll(node.m_leftChild);
        heapifyAll(node.m_rightChild);

        node.heapValue=node.m_key;


        if(node.m_leftChild!=null)
        {
            if(node.heapValue < node.m_leftChild.heapValue)
                node.heapValue = node.m_leftChild.heapValue;
        }
        if(node.m_rightChild!=null)
        {
            if(node.heapValue < node.m_rightChild.heapValue)
                node.heapValue = node.m_rightChild.heapValue;
        }
    }
    public void inorder(TreeNode node)
    {
        if(node==null)
            return;

        inorder(node.m_leftChild);
        //if(node.m_key!=0)
        //{
        System.out.print(node.m_key + " ");
            //debug.add(node.m_key);
        //}
        inorder(node.m_rightChild);
    }
    public void printTree(TreeNode node, int start, int end, int level)
    {
        if(node==null)
            return;

        printTree(node.m_leftChild, start, end, level+1);

        if(node.m_leftChild!=null && node.m_rightChild!=null)
            System.out.println( "[ KEY " +node.m_key + " HEAP "+node.heapValue + " LEVEL " + level + " ]" + "left " + node.m_leftChild.m_key + " right " + node.m_rightChild.m_key);
        else
            System.out.println( "[ KEY " +node.m_key + " HEAP "+node.heapValue + " LEVEL " + level + " ]");

        printTree(node.m_rightChild, start, end,level+1);
    }

    public boolean getRandomKey()
    {
        int index = (int)(Math.random() *(debug.size()));
        System.out.println(" \n\n searching for " + debug.get(index) + " \n\n ");
        return searchvEB(debug.get(index), 0);
    }
}
