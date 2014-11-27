class TreeKey
{
 public:
  virtual bool compare() = 0;
}

struct TreeNode
{
  TreeNode* left;
  TreeNode* right;
  //TreeNode* parent;
  void* data;
  TreeKey* key;
};

class TreeList
{
 public:
  TreeList();
  ~TreeList();
  
  void addNode(TreeNode* node);
  void delNOde(TreeKey* key);
  
 private:
  void revise();
  
  TreeNode* root;
  size_t length;
  TreeNode* buffer;
};





