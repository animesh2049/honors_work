/**
 * Created by Ziaul on 3/7/2017.
 */
public class TreeNode {
    int m_key;
    int pIndex;
    TreeNode m_leftChild;
    TreeNode m_rightChild;
    TreeNode m_parent;
    int vEBIndex;
    int heapValue;
    int influenceIndex=0;
    public TreeNode(TreeNode n)
    {
        m_key = n.m_key;
        m_leftChild = n.m_leftChild;
        m_rightChild = n.m_rightChild;
        m_parent = n.m_parent;
    }
    public TreeNode(int key)
    {
        m_key = key;
        m_leftChild = null;
        m_rightChild = null;
        m_parent = null;
    }
}
