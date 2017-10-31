/**
 * Created by Ziaul on 3/7/2017.
 */
public class PackedArray {
    TreeNode[] nodes = new TreeNode[Constants._MAX_ARRAY_SIZE];
    int Size;
    int Extent=0;
    double TauUpper=1;
    double TauLower=0.5;
    double PhoUpper=0.25;
    double PhoLower=0.125;

    public PackedArray(int size)
    {
        Size=size-1;
        for(int i=0;i<size-1;i++)
        {
            nodes[i] = new TreeNode(i+1);
            nodes[i].pIndex = i+1;
            nodes[i].m_key=i+1;
            nodes[i].heapValue=-1;
        }
    }
    TreeNode getNode(int index)
    {
        return nodes[index];
    }

    TreeNode[] getNodes()
    {
        return nodes;
    }
    int getVebIndex()
    {
        return 0;
    }
}
