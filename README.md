# This is a *Simple Simulator* of IGCN:Island Locator

## Some questions about the raw version of this algorithm:
- *Q1*: When processing a task {hub0,a0} in which a0 is one of hub0's negihbors, hub0 is also in the adjacent list of a0, and it'll be recognized as a hub, according to the raw algorithm, it'll be added to Hlocal, which is inappropriate here.
  In fact, we should add a `if(a0.neighbor is a hub and not in Hlocal)` logic here to avoid redundant hub0 in Hlocal.
- *Q2*: Consider n0 is one of a0's nebighbors, if no is not in Vlocal but in Vglobal, according to the raw algorithm, first it deletes Vlocal from Vglobal, and then it drops the BFS task but add the {Hlocal, Vlocal} to Lislands.
  In fact, in a parallel hardware platform, this condition indicates that this region which includes n0 **is processing or have been processed** by another engine, and it should not touch it again. However, the nodes that have already visited by this engine and recorded in Vlocal and Vglobal are not been visited by other engines for certain, so why should we remove Vlocal from Vglobal? I think the only thing we should do is to add {Hlocal, Vlocal} to Lislands without remove options here. Or, from another perspective, if we remove Vlocal from Vglobal, then we should not add {Hlocal, Vlocal} to Lislands because of the flowing two main reasons:
  - We remove Vlocal from Vglobal to indicate that this region will be visited by another PE.
  - If we do them both, then we'll find that there exists some nodes which are in Lislands but not in Vglobal, it's obviously not right.
We decided to add but not remove here. 
- *Q3*: The raw algorithm use hub's neighbor as the start point of BFS, however, it only checks if the neighbor's neighbor is in Vglobal, but there is no operation performed here to check if the start point itself has been visited, obviously, if yes, then this task will be thoroughly. So we add the flowing codes:
    ```c++
    if(find(Vglobal.begin(),Vglobal.end(),cur_task.second)!=Vglobal.end()) continue;
    ```
    If we find the start point in Vglobal, that means it has been identified as an island_node, we drop this task directly and wait for the next one.
- *Q4*: In raw algorithm line 17, if |Vlocal|>Cmax after appending current node into Vlocal and Vglobal, then break. It seems that judgement condition here doesn't conform the definition of Cmax, i.e. the max number of node in an island. I believe '>' should be replaced by '==' to satisfy the origin definiton of Cmax.

## Some optimizations based on the raw algorithm:
- *OP1*: We noticed that it'll generate lots of small islands in which there only exists one or two island_nodes except the hubs. This may lead to that the enhancement of the spatial locality will be limited, in the meanwhile, it'll increasing complexity of island management. So we add an `push_and_merge()` func to add islands into Lislands and check if it needs to be merged with other islands according to the minimum number of an island,i.e.Cmin.
- *OP2*：Apparently, organizing vertex in the form of islands can definately improve the spatial locality of island_nodes, however, there still exist two points worth thinking:
    - Which nodes are responsible for updating hubs?
    - What if a node in an island has edegs with other nodes in other different island?

    To tackle these issues, （1)we first propose a novel concept,i.e. the hub_island to update the hubs. (2)Additionally we add a shell for every island. The terminology ‘shell’ we use here is corresponding to the second concern we put forward abobe. <u>If an island was built with some edges 'disconnected' because of the limitation from Cmax,or because some graph regions have benn visited by other PEs yet(corresponding to *breakFlag=true* and *discardFlag=true*, respectively), then we record the nodes which are supposed to be one of the island_nodes of this island but actually in other islands as shell_nodes for this island</u>. In this way, in every GCN layer, each island has only communication with the nodes in the same island. 
    
    **However, this will raise another thorny issue, i.e. how to synchronize these shell_nodes when they are updated by the islands they originally belong to.**(TODO)

<font color="red">1. Since we cannot predict how many "small islands" will be merged to form a big island, what if the number of nodes in the big island becomes larger than the Cmax？
2. Consider that some island_nodes in an island are regareded as shell_nodes of a small island, and they must be merged, then after the merge operation, there will be some nodes both in the island_nodes and shell_nodes. How to eliminate this kind of redundancy？</font>

We found that after the islands build process has finished, the total number of hubs plus island_nodes is smaller than the origin node numbers. Through data analysis, we found that there exists another type of nodes, it has only one neighbor and so as its neighbor. I think it's unavoidably so I do nothing about this.